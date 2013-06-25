#include <cstring>
#include "array.h"

#define INITIAL_SIZE 4

namespace kapusha {
Array::Array(size_t item_size, unsigned reserve)
  : itemSize_(item_size), allocated_(reserve)
  , size_(0), items_(nullptr) {
  if (allocated_ != 0)
    items_ = new char[itemSize_ * allocated_];
}

Array::~Array() {
  delete[] items_;
}

unsigned Array::push_back(const void *items, unsigned count) {
  unsigned old_size = size_;
  insert(size_, items, count);
  return old_size;
}

void Array::checkOverflow(unsigned new_size) {
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

void Array::insert(unsigned index, const void *items, unsigned count) {
  KP_ASSERT(index <= size_);
  unsigned new_size = size_ + count;
  checkOverflow(new_size);
  memmove(items_ + itemSize_ * (index + count),
    items_ + itemSize_ * index, itemSize_ * (size_ - index));
  memcpy(items_ + itemSize_ * index, items, itemSize_ * count);
  size_ = new_size;
}

void Array::erase(unsigned index, unsigned count) {
  KP_ASSERT((index + count) <= size_);
  memmove(items_ + itemSize_ * index, items_ + itemSize_ * (index + count),
    itemSize_ * (size_ - index - count));
  size_ -= count;
}

} // namespace kapusha
