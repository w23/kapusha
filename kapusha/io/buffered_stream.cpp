#include <cstring>
#include "buffered_stream.h"

namespace kapusha {
namespace io {

char buffered_stream_t::read() {
  // enter in readable state
  const char ret = peek();
  ++cursor_;
  // leave in readable state
  if (!left()) produce();
  return ret;
}

buffered_stream_t::status_e buffered_stream_t::advance(size_t bytes) {
  while (bytes >= left()) {
    bytes -= left();
    produce();
  }
  cursor_ += bytes;
  return status_;
}

buffered_stream_t::status_e buffered_stream_t::read(void *dst, size_t bytes) {
  char *d = reinterpret_cast<char*>(dst);
  for (; bytes > 0;) {
    const size_t readable = (bytes <= left()) ? bytes : left();
    memcpy(d, cursor(), readable);
    advance(readable);
    bytes -= readable;
    d += readable;
  }
  return status_;
}

void buffered_stream_t::produce_zeroes(buffered_stream_t *stream) {
  static const char zero_buffer[256] = { 0 };
  stream->cursor_ = zero_buffer;
  stream->end_ = zero_buffer + sizeof(zero_buffer);
  stream->produce_ = produce_zeroes;
}

void buffered_stream_t::signal_end_and_produce_zeroes(
  buffered_stream_t *stream) {
  stream->status_ = status_e::end;
  produce_zeroes(stream);
}

void buffered_stream_t::signal_error_and_produce_zeroes(
  buffered_stream_t *stream) {
  stream->status_ = status_e::error;
  produce_zeroes(stream);
}

memory_stream_t::memory_stream_t(const void *source, size_t size) {
  cursor_ = reinterpret_cast<const char*>(source);
  end_ = cursor_ + size;
  produce_ = signal_end_and_produce_zeroes;
  status_ = status_e::ok;
}

} // namespace io
} // namespace kapusha
