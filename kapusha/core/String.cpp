#include <cstring>
#include "String.h"

namespace kapusha {
namespace core {

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