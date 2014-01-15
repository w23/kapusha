#pragma once
#include <kapusha/core/types.h>

namespace kapusha {
namespace ooo {

struct tag_t {
  u32 tag;
  enum { InvalidTag = 0xfffffffful };
  explicit inline tag_t(u32 value = InvalidTag) : tag(value) {}
  explicit inline tag_t(const char *s)
    : tag((s[0] << 24) | (s[1] << 24) | (s[2] << 24) | s[3]) {}

  inline operator u32() const { return tag; }
  inline operator bool() const { return tag != InvalidTag; }
  inline bool operator==(tag_t other) const { return other.tag == tag; }
  inline bool operator!=(tag_t other) const { return other.tag != tag; }
  inline bool operator<(tag_t other) const { return other.tag < tag; }
  inline bool operator>(tag_t other) const { return other.tag > tag; }

  inline bool is_valid() const { return tag != InvalidTag; }
}; // struct tag_t

} // namespace ooo
} // namespace kapusha
