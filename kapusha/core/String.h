#pragma once
#include "assert.h"
#include "shared.h"
#include "buffer.h"

namespace kapusha {
/// \brief LOL Immutable string
class String : public Object {
public:
  explicit String(const char *string = nullptr, int length = -1);
  explicit String(const String &string);
  ~String();

  inline bool empty() const { return length_ == 0; }
  inline size_t length() const { return length_; }
  inline const char *str() const { return string_; }
  inline char operator[](size_t index) const {
    KP_ASSERT(index < length_);
    return string_[index];
  }

  bool operator==(const String &other) const;
  bool operator==(const char *other) const;

private: // force immutability
  String &operator=(const String &other);
  String &operator=(const String &&other);
  String &operator=(const char *other);

private:
  void init(const char *source, size_t length);

  /// length of string in chars, not including zero terminator
  size_t length_;

  /// pointer to a zero-terminated string
  const char *string_;

  /// special case of an empty string points here
  static const char *g_empty_;
}; // class String
typedef shared<String> SString;
} // namespace kapusha