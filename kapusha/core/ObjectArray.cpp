#include "ObjectArray.h"

namespace kapusha {
ObjectArray::ObjectArray(size_t reserve) : size_(0), reserved_(0)
  , buffer_(nullptr)  {
  this->reserve(reserve);
}

ObjectArray::~ObjectArray() {
  delete[] buffer_;
}

void ObjectArray::reserve(size_t reserve) {
  if (reserve <= reserved_) return;
  
  reserved_ = reserve;
  SAnything *buf = new SAnything[reserved_];
  for (size_t i = 0; i < size_; ++i)
    buf[i] = std::move(buffer_[i]);
  delete[] buffer_;
  buffer_ = buf;
}

void ObjectArray::push_back(Object *object) {
  if (size_ == reserved_) reserve(reserved_ ? reserved_ * 2 : 2);
  buffer_[size_] = object;
  ++size_;
}

} // namespace kapusha