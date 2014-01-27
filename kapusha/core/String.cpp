#include "String.h"

namespace kapusha {
namespace core {

string_t::string_t(const string_desc_t desc)
  : buf_(desc.length()+1, desc.begin()) {
  buf_.data_as<char>()[buf_.size()-1] = 0;
}

string_t::string_t(const buffer_t &buffer) : buf_(buffer) {}

string_t::string_t(buffer_t &&buffer) : buf_(std::move(buffer)) {}
string_t::string_t(string_t &&string) : buf_(std::move(string.buf_)) {}

string_t string_t::operator+(const string_desc_t other) const {
  buffer_t b(buf_.size() + other.length());
  b.copy(static_cast<size_t>(0), buf_.data(), buf_.size()-1);
  b.copy(buf_.size() - 1, other.begin(), other.length());
  b.data_as<char>()[b.size()-1] = 0;
  return string_t(std::move(b));
}

///////////////////////////////////////////////////////////////////////////////

String::String(const char *string, int length)
  : length_((length < 0) ? (string?strlen(string):0) : length)
  , buffer_(length_ + 1, string) {
  buffer_.data_as<char>()[length_] = 0;
}

String::String(const String &string)
  : length_(string.length_), buffer_(string.buffer_) {}

String::String(const buffer_t &buffer)
  : length_(buffer_.size() - 1), buffer_(buffer) {}

String::String(buffer_t &&buffer)
  : length_(buffer_.size() - 1), buffer_(std::move(buffer)) {}

String::~String() {}

bool String::operator==(const String &other) const {
  return buffer_ == other.buffer_;
}

bool String::operator==(const char *other) const {
  return 0 == strncmp(buffer_.data_as<char>(), other, length_);
}

} // namespace core
} // namespace kapusha
