/// \file Intrusive list of Objects
#pragma once
#include "Object.h"
#include "assert.h"

namespace kapusha {
namespace core {

class ObjectIntrusiveList : public Object {
public:
  class Item {
  protected:
    friend class ObjectIntrusiveList;
    Item *next_, *prev_;

    // it is possible for object to be referenced only from a list
    // the object here is referenced manually on insert/remove
    Object *object_;

    /// \brief Insert this item before the \p before item
    /// \param[in] before Item that will be the previous one for this
    void insert(Item &before);

  public:
    inline Item(Object *obj) : next_(nullptr), prev_(nullptr), object_(obj) {}
    inline ~Item() {
      /// \todo KP_RUNTIME_ASSERT(!next_, "Item cannot belong to any list");
      KP_ASSERT(!next_);
      KP_ASSERT(!prev_);
    }

    /// \brief Insert this item into the end of a list
    inline void insert(ObjectIntrusiveList &list) { insert(*list.end_.prev_); }

    /// \brief Remove this item from a list
    /// \returns previous item
    Item *remove();
  }; // class Item

  struct iterator {
    inline bool operator==(const iterator &other) const {
      return other.item_ == item_;
    }
    inline bool operator!=(const iterator &other) const {
      return other.item_ != item_;
    }

    inline iterator &operator++() { item_ = item_->next_; return *this; }
    inline iterator &operator++(int) { item_ = item_->next_; return *this; }

    inline Object *operator->() const { return item_->object_; }
    template <typename T>
    inline T *as() const { return static_cast<T*>(item_->object_); }

    inline iterator &remove() { item_ = item_->remove(); return *this; }

  protected:
    friend class ObjectIntrusiveList;
    inline iterator(Item *item) : item_(item) {}

  private:
    Item *item_;
  }; // struct iterator

  struct const_iterator {
    inline bool operator==(const const_iterator &other) const {
      return other.item_ == item_;
    }
    inline bool operator!=(const const_iterator &other) const {
      return other.item_ != item_;
    }

    inline const_iterator &operator++() { item_ = item_->next_; return *this; }
    inline const_iterator &operator++(int) {
      item_ = item_->next_; return *this;
    }

    inline Object *operator->() const { return item_->object_; }
    template <typename T>
    inline T *as() const { return static_cast<T*>(item_->object_); }

  protected:
    friend class ObjectIntrusiveList;
    inline const_iterator(const Item *item) : item_(item) {}

  private:
    const Item *item_;
  }; // struct const_iterator

  ObjectIntrusiveList();
  ~ObjectIntrusiveList();

  inline iterator begin() { return iterator(end_.next_); }
  inline iterator end() { return iterator(&end_); }
  inline const_iterator begin() const { return const_iterator(end_.next_); }
  inline const_iterator end() const { return const_iterator(&end_); }

private:
  friend class Item;
  Item end_;
};

} // namespace core
} // namespace kapusha
