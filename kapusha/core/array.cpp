#include <cstring>
#include "array.h"

#define INITIAL_SIZE 4

namespace kapusha {
Array::Array(std::size_t item_size, unsigned reserve) {
  init(item_size, reserve);
}
void Array::init(std::size_t item_size, unsigned reserve) {
  itemSize_ = item_size;
  allocated_ = reserve;
  size_ = 0;
  if (allocated_ != 0)
    items_ = new char[itemSize_ * allocated_];
  else
    items_ = nullptr;
}

Array::~Array() {
  delete[] items_;
}

unsigned Array::push_back(const void *items, unsigned count) {
  unsigned old_size = size_;
  insert(size_, items, count);
  return old_size;
}

void Array::reserve(unsigned new_size) {
  if (new_size <= allocated_) return;
  //! \todo refine this whole mess
  unsigned new_allocated = (allocated_ == 0) ? INITIAL_SIZE: allocated_;
  //! \todo measure linear vs exponential growth
  while (new_allocated < new_size) new_allocated <<= 1;
  char *new_items = new char[itemSize_ * new_allocated];
  memcpy(new_items, items_, itemSize_ * size_);
  delete[] items_;
  allocated_ = new_allocated;
  items_ = new_items;
}

void *Array::alloc(unsigned index, unsigned count) {
  KP_ASSERT(index <= size_);
  unsigned new_size = size_ + count;
  reserve(new_size);
  void *slot = items_ + itemSize_ * index;
  memmove(items_ + itemSize_ * (index + count), slot, itemSize_ * (size_ - index));
  size_ = new_size;
  return slot;
}
void Array::insert(unsigned index, const void *items, unsigned count) {
   memcpy(alloc(index, count), items, itemSize_ * count);
}

void Array::erase(unsigned index, unsigned count) {
  KP_ASSERT((index + count) <= size_);
  memmove(items_ + itemSize_ * index, items_ + itemSize_ * (index + count),
    itemSize_ * (size_ - index - count));
  size_ -= count;
}

} // namespace kapusha
