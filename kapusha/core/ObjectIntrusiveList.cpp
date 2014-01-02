#include "ObjectIntrusiveList.h"

namespace kapusha {
namespace core {

void ObjectIntrusiveList::Item::insert(Item &before) {
  KP_ASSERT(!next_);
  KP_ASSERT(!prev_);

  object_->retain();
  prev_ = &before;
  next_ = before.next_;
  before.next_ = this;
  next_->prev_ = this;
}

ObjectIntrusiveList::Item *ObjectIntrusiveList::Item::remove() {
  KP_ASSERT(next_);
  KP_ASSERT(prev_);

  Item * const ret = prev_;
  prev_->next_ = next_;
  next_->prev_ = prev_;
  next_ = prev_ = nullptr;

  // last action -- this may delete everything
  object_->release();
  return ret;
}

ObjectIntrusiveList::ObjectIntrusiveList() : end_(nullptr) {
  end_.next_ = &end_;
  end_.prev_ = &end_;
}

ObjectIntrusiveList::~ObjectIntrusiveList() {
  for (Item *it = end_.next_; it != &end_; it = it->next_) {
    it->next_ = it->prev_ = nullptr;
    it->object_->release();
  }
}

} // namespace core
} // namespace kapusha
