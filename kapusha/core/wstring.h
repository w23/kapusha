#pragma once
#include "String.h"

namespace kapusha {
namespace core {
namespace windows {

/// \brief char(utf-8) <-> wchar conversion helper
///
/// Internal representation is windows-native wchar_t
class wstring_t {
public:
  /// \todo accept string_desc_t ?
  explicit wstring_t(const wchar_t *string = nullptr, int length = -1);
  explicit wstring_t(const char *string);
  inline ~wstring_t() {}

  inline int length() const { return length_; }
  inline operator const wchar_t*() const {
    return buf_.data_as<wchar_t>();
  }
  operator core::String*() const;

  static core::String *create_string_from_wchar(
    const wchar_t *wstring, int length = -1);
private:
  int length_;
  buffer_t buf_;
}; // class WString

} // namespace windows
} // namespace core
} // namespace kapusha
