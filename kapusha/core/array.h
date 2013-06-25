#pragma once
#include "assert.h"

namespace kapusha {
//! \brief Basic array for mostly POD types
class Array {
public:
  Array(size_t item_size, unsigned reserve = 0);
  ~Array();

  size_t itemSize() const { return itemSize_; }
  unsigned size() const { return size_; }

  unsigned push_back(const void *items, unsigned count = 1);
  void erase(unsigned index, unsigned count = 1);

  //! \todo void insert(unsigned index, const void *items, unsigned count = 1);

  template <typename T>
  inline T *get(unsigned index) {
    KP_ASSERT(sizeof(T) == itemSize_);
    return reinterpret_cast<T*>(items_) + index;
  }

  template <typename T>
  inline const T *get(unsigned index) const {
    KP_ASSERT(sizeof(T) == itemSize_);
    return reinterpret_cast<const T*>(items_) + index;
  }

private:
  size_t itemSize_;
  unsigned allocated_;
  unsigned size_;
  char *items_;
}; // class Array

//! \brief Template wrapper around array for more pleasure
template <typename T> class ArrayOf : public Array {
public:
  inline ArrayOf(unsigned reserve = 0) : Array(sizeof(T), reserve) {}
  inline unsigned push_back(const T* item, unsigned count = 1) {
    return Array::push_back(item, count);
  }
  inline unsigned push_back(const T* begin, const T *end) {
    return Array::push_back(begin, end - begin);
  }
  inline unsigned push_back(const T& item) {
    return Array::push_back(&item, 1);
  }
}; // class ArrayOf
} // namespace kapusha
