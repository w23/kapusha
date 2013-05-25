// kapusha/core
// 2012-2013 (c) Ivan 'w23' Avdeev, me@w23.ru
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
      KP_ASSERT(!next##Name##_); \
      KP_ASSERT(!prev##Name##_); \
      next##Name##_ = member->next##Name##_; \
      prev##Name##_ = member; \
      if (next ##Name##_) next##Name##_->prev##Name##_ = this; \
      member->next##Name##_ = this; \
    } \
    void removeFrom##Name##s() { \
      retain(); \
      if (prev ##Name##_) prev##Name##_->next##Name##_ = next##Name##_; \
      if (next ##Name##_) next##Name##_->prev##Name##_ = prev##Name##_; \
      next##Name##_.reset(); \
      release(); \
    } \
  private: \
    shared<type> next##Name##_; \
    type *prev##Name##_ = 0
