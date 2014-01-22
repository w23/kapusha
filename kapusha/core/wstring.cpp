#define WIN32_LEAN_AND_MEAN 1
#define NOMINMAX 1
#include <windows.h>
#include "wstring.h"

namespace kapusha {
namespace core {
namespace windows {

wstring_t::wstring_t(const wchar_t *string, int length) {
  if (!string) length = 0;
  if (length < 0) length = wcslen(string);

  length_ = length;
  buf_.resize(sizeof(wchar_t) * (length_ + 1));
  memcpy(buf_.data(), string, sizeof(wchar_t) * length_);
  buf_.data_as<wchar_t>()[length_] = 0;
}

wstring_t::wstring_t(const char *string) : length_(0) {
  length_ = ::MultiByteToWideChar(CP_UTF8, 0, string, -1, NULL, 0);
  if (length_ > 0) {
    buf_.resize(sizeof(wchar_t) * length_);
    ::MultiByteToWideChar(CP_UTF8, 0, string, -1,
      buf_.data_as<wchar_t>(), length_);
    --length_; // do not count null char
  }
}

core::String *wstring_t::create_string_from_wchar(
  const wchar_t *wstring_t, int wlength) {
  int length = ::WideCharToMultiByte(CP_UTF8, 0, wstring_t, wlength,
    NULL, 0, NULL, NULL);
  if (length < 1) return new core::String();

  core::buffer_t buffer(length);
  ::WideCharToMultiByte(CP_UTF8, 0, wstring_t, wlength,
    buffer.data_as<char>(), length, NULL, NULL);
  return new core::String(std::move(buffer));
}

} // namespace windows
} // namespace core
} // namespace kapusha