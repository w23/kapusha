#pragma once
#include <kapusha/core/String.h>
#include "windows_inc.h"

namespace kapusha {
namespace sys {
namespace windows {
/// \brief char(utf-8) <-> wchar conversion helper
///
/// Internal representation is windows-native wchar_t
class WString {
public:
  explicit WString(const wchar_t *string = nullptr, int length = -1);
  explicit WString(const char *string);
  ~WString();

  inline int length() const { return length_; }
  inline operator const wchar_t*() const { return string_; }
  operator core::String*() const;

  static core::String *toString(const wchar_t *wstring, int length = -1);
private:
  const wchar_t *string_;
  int length_;

  static const wchar_t *g_empty_;
}; // class WString

} // namespace windows
} // namespace sys
} // namespace kapusha