#pragma once
#include "shared.h"

namespace kapusha {

// A list of simple pointers
struct list {
  list() : end_(nullptr), front_(&end_), back_(&end_) {
    end_.next = end_.prev = &end_;
  }
  
  struct item {
    void *object;
    item *prev, *next;
    inline item(void *obj) : object(obj), prev(nullptr), next(nullptr) {}
  }; // struct item

  struct iterator {
    inline iterator(item *item) : item_(item) {}
    inline iterator operator++() {
      item_ = item_->next;
      return *this;
    }
    inline bool operator==(iterator r) const { return item_ == r.item_; }
    inline bool operator!=(iterator r) const { return !(*this == r); }
    inline bool operator==(const void *r) const { return item_->object == r; }
    inline bool operator!=(const void *r) const { return !(*this == r); }
    template <typename T> T *get() const {
      return reinterpret_cast<T*>(item_->object);
    }
  protected:
    item *item_;
  }; // struct iterator
  
  inline iterator begin() { return iterator(front_); }
  inline iterator end() { return iterator(&end_); }

  void insert(iterator before, void *object);
  inline void insert_back(void *object) { insert(end(), object); }
  iterator remove(iterator it);

private:
  item end_;
  item *front_, *back_;
}; // struct list
} // namespace kapusha
