#pragma once
#include <kapusha/core/Array.h>
#include "Tag.h"

namespace kapusha {
namespace ooo {

/// this is for very small array sizes (~<10..20)
class tagged_array_t {
public:
  inline tagged_array_t(u32 item_size, u32 reserve = 0,
    core::array_t::item_dtor_f item_dtor = nullptr)
    : array_(item_size + sizeof(tag_t), reserve, item_dtor) {}

  const void *operator[](tag_t tag) const;
  void *operator[](tag_t tag);

  struct item_t {
    tag_t tag;
    void *item;
    item_t(tag_t tag_in, void *item_in) : tag(tag_in), item(item_in) {}
  };
  struct const_item_t {
    tag_t tag;
    const void *item;
    const_item_t(tag_t tag_in, const void *item_in)
      : tag(tag_in), item(item_in) {}
  };

  u32 size() const { return array_.size(); }
  const_item_t operator[](u32 index) const;
  item_t operator[](u32 index);

  void *set(tag_t tag, const void *source_item = nullptr);
private:
  struct item_header_t {
    tag_t tag;
    char data[];
  /*private:
    item_header_t(const item_header_t &other);
    item_header_t & operator=(const item_header_t &other);*/
  };
  const item_header_t *find(tag_t tag) const;

  core::array_t array_;
};

template <typename T> class tagged_array_of_t : public tagged_array_t {
public:
  inline tagged_array_of_t(u32 reserve = 0)
    : tagged_array_t(sizeof(T), reserve, item_dtor) {}
  inline T &set(tag_t tag, const T *source_item = nullptr) {
    return *reinterpret_cast<T*>(tagged_array_t::set(tag, source_item));
  }
  inline const T *operator[](tag_t tag) const {
    return reinterpret_cast<const T*>(tagged_array_t::operator[](tag));
  }
  void *operator[](tag_t tag) {
    return reinterpret_cast<T*>(tagged_array_t::operator[](tag));
  }

  struct item_t {
    tag_t tag;
    T *item;
  protected:
    friend class tagged_array_of_t;
    inline item_t(const tagged_array_t::item_t &it) : tag(it.tag)
      , item(reinterpret_cast<T*>(it.item)) {}
  };
  struct const_item_t {
    tag_t tag;
    const T *item;
  protected:
    friend class tagged_array_of_t;
    inline const_item_t(const tagged_array_t::const_item_t &it) : tag(it.tag)
      , item(reinterpret_cast<const T*>(it.item)) {}
  };
  inline const_item_t operator[](u32 index) const {
    return const_item_t(tagged_array_t::operator[](index));
  }
  inline item_t operator[](u32 index) {
    return item_t(tagged_array_t::operator[](index));
  }

private:
  static void item_dtor(void *item_ptr) {
    T *item = reinterpret_cast<T*>(item_ptr);
    item->~T();
  }
};

} // namespace ooo
} // namespace kapusha
