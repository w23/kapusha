#include "list.h"

namespace kapusha {
  
void core::list::insert(iterator before, void *object) {
  item *it = new item(object);
  it->next = before.item_;
  it->prev = before.item_->prev;
  it->prev->next = it;
  it->next->prev = it;
  
  if (before == front_) front_ = it;
  if (before == back_) back_ = it;
}

  core::list::iterator core::list::remove(iterator it) {
  iterator ret(it.item_->next);
  it.item_->next->prev = it.item_->prev;
  it.item_->prev->next = it.item_->next;
  delete it.item_;
  return ret;
}

} // namespace kapusha
