#pragma once
#include "shared.h"
#include "assert.h"

namespace kapusha {
namespace core {

/// \brief Dynamic array of Objects
class ObjectArray : public Object {
public:
  typedef core::shared<ObjectArray> shared;
  ObjectArray(size_t reserve = 0);
  ~ObjectArray();

  inline size_t size() const { return size_; }
  inline Object *operator[](size_t index) const {
    KP_ASSERT(index < size_);
    return buffer_[index];
  }

  void reserve(size_t reserve);
  void push_back(Object *object);

private:
  size_t size_, reserved_;
  SAnything *buffer_;
}; // class ObjectArray

/// \brief Dynamic array of typed objects (syntax sugar)
template <typename T> class ObjectArrayOf : public ObjectArray {
public:
  inline ObjectArrayOf(size_t reserve = 0) : ObjectArray(reserve) {}
  inline T *operator[](size_t index) const {
    return static_cast<T*>(ObjectArray::operator[](index));
  }
}; // class ObjectArrayOf

} // namespace core
} // namespace kapusha
