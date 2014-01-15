#pragma once
#include "types.h"
#include "assert.h"
#include "buffer.h"

namespace kapusha {
namespace core {

//! \brief Basic array for mostly POD types
class array_t {
public:
#if !defined(_MSC_VER)
  inline array_t() = default;
#else
  inline array_t() {}
#endif

  typedef void (*item_dtor_f)(void *item);

  array_t(u32 item_size, u32 reserve = 0, item_dtor_f item_dtor = nullptr);
  ~array_t();

  inline u32 item_size() const { return item_size_; }
  inline item_dtor_f item_dtor() const { return item_dtor_; }
  inline u32 size() const { return size_; }
  inline u32 total_size_in_bytes() const { return size() * item_size(); }

  void clear();

  void reserve(u32 items);
  void resize(u32 items);

  inline void *alloc_back(u32 count = 1) {
    return alloc(size_, count);
  }
  u32 push_back(const void *items, u32 count = 1);
  void erase(u32 index, u32 count = 1);

  void *alloc(u32 index, u32 count = 1);
  void insert(u32 index, const void *items, u32 count = 1);

  inline const void *operator[](u32 index) const {
    KP_ASSERT(index < size_);
    return buffer_.data() + item_size_ * index;
  }

  inline void *operator[](u32 index) {
    KP_ASSERT(index < size_);
    return buffer_.data() + item_size_ * index;
  }

  template <typename T>
  inline T *get(u32 index) {
    KP_ASSERT(sizeof(T) == item_size_);
    KP_ASSERT(index < size_);
    return reinterpret_cast<T*>(buffer_.data()) + index;
  }

  template <typename T>
  inline const T *get(u32 index) const {
    KP_ASSERT(sizeof(T) == item_size_);
    KP_ASSERT(index < size_);
    return reinterpret_cast<const T*>(buffer_.data()) + index;
  }

private:
  u32 item_size_;
  u32 size_, reserved_;
  item_dtor_f item_dtor_;
  buffer_t buffer_;
}; // class array_t

//! \brief Template wrapper around array for more pleasure
template <typename T> class array_of_t : public array_t {
public:
  inline array_of_t(unsigned reserve = 0) : array_t(sizeof(T), reserve, item_dtor) {}
  inline unsigned push_back(const T* item, unsigned count = 1) {
    return array_t::push_back(item, count);
  }
  inline unsigned push_back(const T* begin, const T *end) {
    return array_t::push_back(begin, end - begin);
  }
  inline unsigned push_back(const T& item) {
    return array_t::push_back(&item, 1);
  }
  inline void insert(unsigned index, const T& item) {
    array_t::insert(index, &item, 1);
  }
  const T& operator[](unsigned index) const { return *get<T>(index); }
  T& operator[](unsigned index) { return *get<T>(index); }

private:
  static void item_dtor(void *item_ptr) {
    T *item = reinterpret_cast<T*>(item_ptr);
    item->~T();
  }
}; // class array_of_t

} // namespace core
} // namespace kapusha
