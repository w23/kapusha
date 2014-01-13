#include "extern.h"
#include "Array.h"

namespace kapusha {
namespace core {
array_t::array_t(u32 item_size, u32 reserve) : item_size_(item_size), size_(0)
  , reserved_(reserve), buffer_(item_size * reserve) {
}

array_t::~array_t() {}

u32 array_t::push_back(const void *items, u32 count) {
  u32 old_size = size_;
  insert(size_, items, count);
  return old_size;
}

void array_t::reserve(u32 items) {
  if (items <= reserved_) return;
  reserved_ = items;
  buffer_.resize(reserved_ * item_size_);
}

void array_t::resize(u32 items) {
  if (items > reserved_) {
    u32 reserved = reserved_ ? reserved_ : 1;
    while (reserved < items) reserved <<= 1;
    reserve(reserved);
  }
  size_ = items;
}

void *array_t::alloc(u32 index, u32 count) {
  KP_ASSERT(index <= size_);
  u32 new_size = size_ + count;
  reserve(new_size);
  void *slot = buffer_.data() + item_size_ * index;
  memmove(buffer_.data() + item_size_ * (index + count), slot,
    item_size_ * (size_ - index));
  size_ = new_size;
  return slot;
}
void array_t::insert(u32 index, const void *items, u32 count) {
   memcpy(alloc(index, count), items, item_size_ * count);
}

void array_t::erase(u32 index, u32 count) {
  KP_ASSERT((index + count) <= size_);
  memmove(buffer_.data() + item_size_ * index,
    buffer_.data() + item_size_ * (index + count),
    item_size_ * (size_ - index - count));
  size_ -= count;
}
} // namespace core
} // namespace kapusha
