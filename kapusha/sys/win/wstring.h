#pragma once
#include "windows_inc.h"

namespace kapusha {
  class wstring {
  public:
    wstring(const char *string) : wstring_(nullptr), size_(0) {
      size_ = MultiByteToWideChar(CP_UTF8, 0, string, -1, NULL, 0);
      if (size_ > 0) {
        wstring_ = new wchar_t[size_];
        MultiByteToWideChar(CP_UTF8, 0, string, -1, wstring_, size_);
        --size_; // do not count null char
      }
    }
    ~wstring() { delete [] wstring_; }

    int size() const { return size_; }
    operator const wchar_t*() const { return wstring_; }
  private:
    wchar_t *wstring_;
    int size_;
  };
} // namespace kapusha