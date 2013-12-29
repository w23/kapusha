#include "WString.h"

namespace kapusha {
const wchar_t *WString::g_empty_ = L"";

WString::WString(const wchar_t *string, int length) {
  if (!string) length = 0;
  if (length < 0) length = wcslen(string);
  if (length == 0) {
    string_ = g_empty_;
    length_ = 0;
    return;
  }

  length_ = length;
  wchar_t *buf = new wchar_t[length_ + 1];
  memcpy(buf, string, sizeof(wchar_t) * length_);
  buf[length_] = 0;
  string_ = buf;
}

WString::WString(const char *string) : string_(g_empty_), length_(0) {
  length_ = MultiByteToWideChar(CP_UTF8, 0, string, -1, NULL, 0);
  if (length_ > 0) {
    wchar_t *string_buf = new wchar_t[length_];
    MultiByteToWideChar(CP_UTF8, 0, string, -1, string_buf, length_);
    string_ = string_buf;
    --length_; // do not count null char
  }
}

WString::~WString() { if (length_ != 0) delete [] string_; }

String *WString::toString(const wchar_t *wstring, int wlength) {
  int length = WideCharToMultiByte(CP_UTF8, 0, wstring, wlength, NULL, 0, NULL, NULL);
  if (length < 1) return new String();

  buffer_t buffer(length);
  WideCharToMultiByte(CP_UTF8, 0, wstring, wlength, buffer.data_as<char>(), length, NULL, NULL);
  return new String(std::move(buffer));
}

} // namespace kapusha