#pragma once
#include "assert.h"
#include "shared.h"
#include "buffer.h"
#include "ObjectArray.h"

namespace kapusha {
namespace core {

/// \brief LOL Immutable string
class String : public Object {
public:
  typedef shared<String> shared;
  explicit String(const char *string = nullptr, int length = -1);
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