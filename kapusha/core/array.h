#pragma once
#include <cstring>
#include "types.h"
#include "assert.h"

namespace kapusha {
//! \brief Basic array for mostly POD types
class Array {
public:
#if !defined(_MSC_VER)
  inline Array() = default;
#else
  inline Array() {}
#endif
  Array(std::size_t item_size, unsigned reserve = 0);
  ~Array();
  
  void init(std::size_t item_size, unsigned reserve = 0);

  std::size_t itemSize() const { return itemSize_; }
  unsigned size() const { return size_; }
  std::size_t totalSizeInBytes() const { return size() * itemSize(); }
  inline void clear() { size_ = 0; }
  /// \todo void distClear() { delete[] items_ ... }
  
  void reserve(unsigned new_size);
  inline void resize(u32 size) {
    reserve(size);
    size_ = size;
  }

  inline void *alloc_back(unsigned count = 1) {
    return alloc(size_, count);
  }
  unsigned push_back(const void *items, unsigned count = 1);
  void erase(unsigned index, unsigned count = 1);

  void *alloc(unsigned index, unsigned count = 1);
  void insert(unsigned index, const void *items, unsigned count = 1);

  inline const void *get(unsigned index) const {
    KP_ASSERT(index < size_);
    return reinterpret_cast<u8*>(items_) + itemSize_ * index;
  }

  inline void *get(unsigned index) {
    KP_ASSERT(index < size_);
    return reinterpret_cast<u8*>(items_) + itemSize_ * index;
  }

  template <typename T>
  inline T *get(unsigned index) {
    KP_ASSERT(sizeof(T) == itemSize_);
    KP_ASSERT(index < size_);
    return reinterpret_cast<T*>(items_) + index;
  }

  template <typename T>
  inline const T *get(unsigned index) const {
    KP_ASSERT(sizeof(T) == itemSize_);
    KP_ASSERT(index < size_);
    return reinterpret_cast<const T*>(items_) + index;
  }

private:
  std::size_t itemSize_;
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
  inline void insert(unsigned index, const T& item) {
    Array::insert(index, &item, 1);
  }
  const T& operator[](unsigned index) const { return *get<T>(index); }
  T& operator[](unsigned index) { return *get<T>(index); }
}; // class ArrayOf
} // namespace kapusha
