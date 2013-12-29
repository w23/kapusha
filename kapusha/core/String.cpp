#include <cstring>
#include "String.h"

namespace kapusha {
const char *String::g_empty_ = "";

String::String(const char *string, int length) {
  if (!string) length = 0;
  if (length < 0)
    init(string, strlen(string));
  else
    init(string, length);
}

String::String(const String &string) {
  init(string.string_, string.length_);
}

void String::init(const char *source, size_t length) {
  if (length == 0) {
    length_ = 0;
    string_ = g_empty_;
    return;
  }

  length_ = length;
  char *string = new char[length_ + 1];
  memcpy(string, source, length_);
  string[length_] = 0; // source string can be non-zero-terminated
  string_ = string;
}

String::~String() {
  if (length_) delete[] string_;
}

bool String::operator==(const String &other) const {
  if (length_ != other.length_) return false;
  return 0 == memcmp(string_, other.string_, length_);
}

bool String::operator==(const char *other) const {
  return 0 == strncmp(string_, other, length_);
}

} // namespace kapusha