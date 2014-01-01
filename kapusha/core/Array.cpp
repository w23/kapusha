#include "extern.h"
#include "Array.h"

namespace kapusha {
namespace core {
Array::Array(u32 item_size, u32 reserved) {
  init(item_size, reserved);
}

void Array::init(u32 item_size, u32 reserved) {
  item_size_ = item_size;
  reserved_ = 0;
  size_ = 0;
  items_ = nullptr;
  reserve(reserved);
}

Array::~Array() {
  delete[] items_;
}

u32 Array::push_back(const void *items, u32 count) {
  u32 old_size = size_;
  insert(size_, items, count);
  return old_size;
}

void Array::reserve(u32 reserved) {
  if (reserved <= reserved_) return;

  u8 *items = new u8[item_size_ * reserved];
  memcpy(items, items_, item_size_ * size_);
  delete[] items_;
  reserved_ = reserved;
  items_ = items;
}

void Array::resize(u32 size) {
  if (size > reserved_) {
    u32 reserved = reserved_ ? reserved_ : 1;
    while (reserved < size) reserved <<= 1;
    reserve(reserved);
  }
  size_ = size;
}

void *Array::alloc(u32 index, u32 count) {
  KP_ASSERT(index <= size_);
  u32 new_size = size_ + count;
  reserve(new_size);
  void *slot = items_ + item_size_ * index;
  memmove(items_ + item_size_ * (index + count), slot, item_size_ * (size_ - index));
  size_ = new_size;
  return slot;
}
void Array::insert(u32 index, const void *items, u32 count) {
   memcpy(alloc(index, count), items, item_size_ * count);
}

void Array::erase(u32 index, u32 count) {
  KP_ASSERT((index + count) <= size_);
  memmove(items_ + item_size_ * index, items_ + item_size_ * (index + count),
    item_size_ * (size_ - index - count));
  size_ -= count;
}
} // namespace core
} // namespace kapusha
