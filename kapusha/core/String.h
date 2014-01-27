#pragma once
#include "assert.h"
#include "shared.h"
#include "buffer.h"
#include "ObjectArray.h"

namespace kapusha {
namespace core {

/// \brief Immutable temporary string descriptor, may be not zero-terminated
///
/// \warning Does not manage string buffer, temp/volatile usage only
struct string_desc_t {
  string_desc_t() = default;
  string_desc_t(const char *string) : begin_(string) {
    KP_ASSERT(string);
    end_ = begin_ + strlen(string);
  }
  inline string_desc_t(const char *begin, const char *end,
    bool zero_terminated = false)
    : begin_(begin), end_(end), zero_terminated_(zero_terminated) {}
  inline string_desc_t(const char *begin, size_t length,
    bool zero_terminated = false)
    : string_desc_t(begin, begin+length, zero_terminated) {}

  inline size_t length() const { return end_ - begin_; }
  inline operator bool() const { return !!begin_; }
  inline const char *begin() const { return begin_; }
  inline const char *end() const { return end_; }

  inline bool is_zero_terminated() const { return zero_terminated_; }
private:
  const char *begin_, *end_;
  bool zero_terminated_;
};

struct string_t {
  string_t(const string_desc_t desc);
  string_t(string_t &&string);
  string_t(const buffer_t &buffer);
  string_t(buffer_t &&buffer);

  inline const char *str() const { return buf_.data_as<char>(); }
  inline size_t length() const { return buf_.size() - 1; }

  inline operator const char*() const { return str(); }
  inline operator string_desc_t() const {
    return string_desc_t(str(), length());
  }

  string_t operator+(const string_desc_t other) const;
private:
  buffer_t buf_;
};

/// \brief LOL Immutable string
class String : public Object {
public:
  typedef core::shared<String> shared;
  explicit String(const char *string = nullptr, int length = -1);
  explicit inline String(const string_desc_t &desc)
    : String(desc.begin(), desc.length()) {}
  explicit String(const String &string);

  /// \warning valid non-zero length and zero-terminated buffer expected
  explicit String(const buffer_t &buffer);

  /// \warning valid non-zero length and zero-terminated buffer expected
  explicit String(buffer_t &&buffer);

  ~String();

  inline bool empty() const { return length_ == 0; }
  inline size_t length() const { return length_; }
  inline const char *str() const { return buffer_.data_as<char>(); }
  inline char operator[](size_t index) const {
    KP_ASSERT(index < buffer_.size());
    return buffer_.data_as<char>()[index];
  }
  inline const buffer_t &buffer() const { return buffer_; }
  inline operator const char*() const { return str(); }
  inline operator const buffer_t&() const { return buffer_; }
  inline operator const buffer_t*() const { return &buffer_; }
  inline operator const string_desc_t() const {
    return string_desc_t(buffer_.data_as<char>(), length_, true);
  }

  bool operator==(const String &other) const;
  bool operator==(const char *other) const;

private: // force immutability
  String &operator=(const String &other);
  String &operator=(const String &&other);

private:
  void init(const char *source, size_t length);
  size_t length_;
  buffer_t buffer_;
}; // class String
typedef shared<String> SString;

class StringArray : public ObjectArrayOf<String> {
public:
  typedef ObjectArrayOf<String> base;
  typedef core::shared<StringArray> shared;
  StringArray(size_t reserved = 0) : base(reserved) {}
};

} // namespace core
} // namespace kapusha
