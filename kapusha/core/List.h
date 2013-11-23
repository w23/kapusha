/// Intrusive lists

#pragma once
#include "shared.h"

//! \warning next link is strong
//! prev link is weak
//! loops are not supported
#define KP_DECLARE_DLINKEDLIST(type, Name) \
  public: \
    inline type *getNext##Name() { return next##Name##_.get(); } \
    inline type *getPrev##Name() { return prev##Name##_; } \
    void insertAfter##Name(type *member) { \
      KP_ASSERT(member); \
      KP_ASSERT(!next##Name##_.valid()); \
      KP_ASSERT(!prev##Name##_); \
      next##Name##_ = member->next##Name##_; \
      prev##Name##_ = member; \
      if (next ##Name##_.valid()) next##Name##_->prev##Name##_ = this; \
      member->next##Name##_ = this; \
    } \
    void removeFrom##Name##s() { \
      retain(); \
      if (prev ##Name##_) prev##Name##_->next##Name##_ = next##Name##_; \
      if (next ##Name##_.valid()) next##Name##_->prev##Name##_ = prev##Name##_; \
      next##Name##_.reset(); \
      release(); \
    } \
  private: \
    shared<type> next##Name##_; \
    type *prev##Name##_ //! \todo fuck msvc = 0

/// \brief Helper linkage struct, constains core logic
template <typename T, typename Name>
struct IntrusiveListItem {
  IntrusiveListItem() : next_(nullptr), prev_(nullptr) {}
  inline T *next() const { return next_; }
  inline T *prev() const { return prev_; }
  
  void insert_after(T *after) {
    Name T::*l;
    prev_ = after;

    if (prev_) {
      next_ = prev_->*l.next_;
      prev_->*l.next_ = this;
    }
    
    if (next_) next_->*l.prev_ = this;
  }

  /// \returns next item
  T *remove() {
    Name T::*l;
    if (next_) next_->*l.prev_ = prev_;
    if (prev_) prev_->*l.next_ = next_;
    next_ = prev_ = nullptr;
  }

private:
  T *next_, *prev_;
};

/// \brief Mostly empty header for a pointer-only non-owning double-linked list
///
/// Works over objects that have IntrusiveListItem fields named <ItemName>
template <typename T, typename ItemName>
struct IntrusiveList {
  struct iterator {
    explicit inline iterator(T *obj) : obj_(obj) {}
    inline iterator &operator++() { obj_ = obj_->ItemName.next(); return *this; }
    inline T *operator->() { return obj_; }
    inline T *operator*() { return obj_; }

  protected:
    T *obj_;
  };

  inline iterator *begin() { return iterator(front_); }
  inline iterator *end() { return iterator(nullptr); }

  void insert_back(T *obj) {
    ItemName T::*i;
    obj->*i.insert_after(back_);
    if (front_ == nullptr)
      front_ = back_ = obj;
  }
  iterator remove(iterator it) {
    ItemName T::*i;
    if (it.obj_ == front_) front_->*i.next();
    if (it.obj_ == back_) back_->*i.prev();
    return iterator(it.obj_->*i.remove());
  }

  IntrusiveList() : front_(nullptr), back_(nullptr) {}

private:
  T *front_, *back_;
};
