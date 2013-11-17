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
  Array(u32 item_size, u32 reserve = 0);
  ~Array();
  
  void init(u32 item_size, u32 reserve = 0);

  inline u32 item_size() const { return item_size_; }
  inline u32 size() const { return size_; }
  inline u32 total_size_in_bytes() const { return size() * item_size(); }
  inline void clear() { size_ = 0; }
  
  void reserve(u32 items);
  void resize(u32 items);

  inline void *alloc_back(u32 count = 1) {
    return alloc(size_, count);
  }
  u32 push_back(const void *items, u32 count = 1);
  void erase(u32 index, u32 count = 1);

  void *alloc(u32 index, u32 count = 1);
  void insert(u32 index, const void *items, u32 count = 1);

  inline const void *get(u32 index) const {
    KP_ASSERT(index < size_);
    return items_ + item_size_ * index;
  }

  inline void *get(u32 index) {
    KP_ASSERT(index < size_);
    return items_ + item_size_ * index;
  }

  template <typename T>
  inline T *get(u32 index) {
    KP_ASSERT(sizeof(T) == item_size_);
    KP_ASSERT(index < size_);
    return reinterpret_cast<T*>(items_) + index;
  }

  template <typename T>
  inline const T *get(unsigned index) const {
    KP_ASSERT(sizeof(T) == item_size_);
    KP_ASSERT(index < size_);
    return reinterpret_cast<const T*>(items_) + index;
  }

private:
  u32 item_size_;
  u32 reserved_;
  u32 size_;
  u8 *items_;
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
