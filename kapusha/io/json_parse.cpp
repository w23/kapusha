#include <kapusha/core/extern.h>
#include "json_parse.h"

namespace kapusha {
namespace io {

struct consum_t {
  inline consum_t(json_t::consumer_t *consumer) : consumer_(consumer) {}

  inline bool value(core::string_t &&string) {
    return consumer_->value_string(consumer_, std::move(string));
  }
  inline bool value(int i) { return consumer_->value_int(consumer_, i); }
  inline bool value(float f) {
    return consumer_->value_float(consumer_, f);
  }
  inline bool value(bool b) {
    return consumer_->value_bool(consumer_, b);
  }
  inline bool value_null() { return consumer_->value_null(consumer_); }
  inline bool array_begin() {
    return consumer_->value_array(consumer_, true);
  }
  inline bool array_end() {
    return consumer_->value_array(consumer_, false);
  }
  inline bool object_begin() {
    return consumer_->value_object(consumer_, true);
  }
  inline bool object_end() {
    return consumer_->value_object(consumer_, false);
  }

private:
  json_t::consumer_t *consumer_;
};

enum class token_e {
  invalid,
  curly_open, curly_close,
  square_open, square_close,
  comma, colon, quote,
  lit_number,
  lit_true, lit_false, lit_null
};

struct unescaped_string_stream_t : public buffered_stream_t {
  unescaped_string_stream_t(buffered_stream_t &source)
    : source_(source), advance_(0) {
    produce_ = produce_restream;
    produce_(this);
  }

private:
  buffered_stream_t &source_;
  size_t advance_;
  char buffer_[4];

  static void produce_restream(buffered_stream_t *stream) {
    unescaped_string_stream_t *self
      = reinterpret_cast<unescaped_string_stream_t*>(stream);
    buffered_stream_t &src = self->source_;

    src.advance(self->advance_);
    self->advance_ = 0;
    self->cursor_ = src.cursor();
    self->status_ = src.status();
    const char *end = self->cursor_;

    for (; end != src.end(); ++end) {
      const char c = *end;
      if (c == '\\') {
        self->produce_ = produce_unescape;
        break;
      }
      if (c == '"') {
        self->produce_ = signal_end;
        break;
      }
      if (static_cast<unsigned char>(c) < 0x20) {
        self->produce_ = signal_error;
        break;
      }
    }

    self->end_ = end;
    self->advance_ = self->left();

    // if nothing to restream
    if (!self->left())
      self->produce();
  }

  static void produce_unescape(buffered_stream_t *stream) {
    unescaped_string_stream_t *self
      = reinterpret_cast<unescaped_string_stream_t*>(stream);
    buffered_stream_t &src = self->source_;

    src.advance(self->advance_ + 1 /* \ */);
    self->advance_ = 0;

    const char c = src.read();
    switch(c) {
    case '"': self->buffer_[0] = '"'; break;
    case '\\': self->buffer_[0] = '\\'; break;
    case '/': self->buffer_[0] = '/'; break;
    case 'b': self->buffer_[0] = '\b'; break;
    case 'f': self->buffer_[0] = '\f'; break;
    case 'n': self->buffer_[0] = '\n'; break;
    case 'r': self->buffer_[0] = '\r'; break;
    case 't': self->buffer_[0] = '\t'; break;
    // case 'u': utf-16+surrogate pairs to utf-8 conv is horrible, i refuse
    default:
      signal_error_and_produce_zeroes(self);
      return;
    }

    self->cursor_ = self->buffer_;
    self->end_ = self->buffer_ + 1;
    self->produce_ = produce_restream;
  }

  static void signal_end(buffered_stream_t *stream) {
    unescaped_string_stream_t *self
      = reinterpret_cast<unescaped_string_stream_t*>(stream);
    buffered_stream_t &src = self->source_;
    src.advance(self->advance_ + 1 /* " */);
    signal_end_and_produce_zeroes(self);
  }

  static void signal_error(buffered_stream_t *stream) {
    unescaped_string_stream_t *self
      = reinterpret_cast<unescaped_string_stream_t*>(stream);
    buffered_stream_t &src = self->source_;
    src.advance(self->advance_);
    signal_error_and_produce_zeroes(self);
  }
};

static void skip_whitespace(buffered_stream_t &stream) {
  while(isspace(stream.peek())) stream.read();
}

static bool match(buffered_stream_t &stream, const char *str) {
 for (; *str != 0; ++str, stream.read()) {
  if (stream.status() != buffered_stream_t::status_e::ok
    || *str != stream.peek())
    return false;
 }
 return true;
}

static bool read_value(buffered_stream_t &stream, consum_t consum);
static bool read_value(token_e tok, buffered_stream_t &stream, consum_t consum);

static token_e next_token(buffered_stream_t &stream) {
  skip_whitespace(stream);
  if (stream.status() != buffered_stream_t::status_e::ok)
    return token_e::invalid;
  const char c = stream.peek();
  token_e ret = token_e::invalid;
  switch (c) {
  case '{': ret = token_e::curly_open; break;
  case '}': ret = token_e::curly_close; break;
  case '[': ret = token_e::square_open; break;
  case ']': ret = token_e::square_close; break;
  case '"': ret = token_e::quote; break;
  case ':': ret = token_e::colon; break;
  case ',': ret = token_e::comma; break;
  case 't': if (match(stream,"true")) return token_e::lit_true; break;
  case 'f': if (match(stream,"false")) return token_e::lit_false; break;
  case 'n': if (match(stream,"null")) return token_e::lit_null; break;
  default:
    if (c == '-' || (c >= '0' && c <= '9'))
      return token_e::lit_number;
  }
  stream.read();
  return ret;
}

static bool read_string(buffered_stream_t &stream, consum_t consum) {
  unescaped_string_stream_t ss(stream);
  core::buffer_t str;
  while (ss.status() == buffered_stream_t::status_e::ok) {
    str.append(ss.cursor(), ss.left());
    ss.produce();
  }
  if (ss.status() == buffered_stream_t::status_e::error) return false;
  char zero = 0;
  str.append(&zero, 1);
  return consum.value(core::string_t(std::move(str)));
}

static int read_decimal_int(buffered_stream_t &stream, int &digits) {
  int sign = 1;
  if (stream.peek() == '-') {
    sign = -1;
    stream.read();
  } else if (stream.peek() == '+') {
    stream.read();
  }

  int val = 0;
  digits = 0;
  for (;;) {
    const char c = stream.peek();
    if ('0' <= c && c <= '9') {
      val *= 10;
      val += c - '0';
    } else break;
    ++digits;
    stream.read();
  }
  return val * sign;
}

/// \todo fix this whole number parsing mess

static bool read_number(buffered_stream_t &stream, consum_t consum) {
  int digits = 0;
  int ival = read_decimal_int(stream, digits);
  if (!digits || digits > 9) return false;

  if (stream.peek() == '.') {
    stream.read();
    float fval = static_cast<float>(ival);

    /// \todo fix accepting corrupted numbers like 23.-17
    float fract = static_cast<float>(read_decimal_int(stream, digits));
    if (!digits || digits > 9) return false;
    while (--digits >= 0) fract *= .1f;
    fval += fract;

    const char c = stream.peek();
    if (c == 'e' || c == 'E') {
      stream.read();
      int e = read_decimal_int(stream, digits);
      float fexp = 1.;
      while (--e >= 0) fexp *= 10.f;
      fval *= fexp;
    }
    consum.value(fval);
  } else
    consum.value(ival);

  return true;
}

static bool read_object(buffered_stream_t &stream, consum_t consum) {
  consum.object_begin();
  token_e tok = next_token(stream);
  while (tok != token_e::curly_close) {
    if (tok != token_e::quote) return false;
    if (!read_string(stream, consum)) return false;
    if (next_token(stream) != token_e::colon) return false;
    if (!read_value(stream, consum)) return false;
    tok = next_token(stream);
    if (tok == token_e::comma) tok = next_token(stream);
    else if (tok != token_e::curly_close) return false;
  }
  return consum.object_end();
}

static bool read_array(buffered_stream_t &stream, consum_t consum) {
  consum.array_begin();
  token_e tok = next_token(stream);
  while (tok != token_e::square_close) {
    if (!read_value(tok, stream, consum)) return false;
    tok = next_token(stream);
    if (tok == token_e::comma) tok = next_token(stream);
    else if (tok != token_e::square_close) return false;
  }
  return consum.array_end();
}

static bool read_value(buffered_stream_t &stream, consum_t consum) {
  return read_value(next_token(stream), stream, consum);
}

static bool read_value(token_e tok, buffered_stream_t &stream, consum_t consum) {
  switch (tok) {
  case token_e::quote: return read_string(stream, consum);
  case token_e::curly_open: return read_object(stream, consum);
  case token_e::square_open: return read_array(stream, consum);
  case token_e::lit_number: return read_number(stream, consum);
  case token_e::lit_true: return consum.value(true);
  case token_e::lit_false: return consum.value(false);
  case token_e::lit_null: return consum.value_null();
  default: break;
  }
  return false;
}

bool json_t::parse(buffered_stream_t &stream, consumer_t *consumer) {
  consum_t consum(consumer);
  return read_value(stream, consum);
}

} // namespace io
} // namespace kapusha
