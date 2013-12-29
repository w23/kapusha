#pragma once
#include "Object.h"

namespace kapusha {
class shared_base {
public:
  inline shared_base(Object *t = nullptr) : t_(t) { if (t_) t_->retain(); }
  inline shared_base(const shared_base &other)
    : t_(other.t_) { if (t_) t_->retain(); }
  inline shared_base(shared_base &&other)
    : t_(other.t_) { other.t_ = nullptr; }
  inline ~shared_base() { if (t_) t_->release(); }
  void reset(Object* r = nullptr);
  inline bool valid() const { return t_ != nullptr; }
  inline shared_base &operator=(Object *t) { reset(t); return *this; }
  inline shared_base &operator=(const shared_base &other) {
    reset(other.t_);
    return *this;
  }
  inline shared_base &operator=(shared_base &&other) {
    t_ = other.t_;
    other.t_ = nullptr;
    return *this;
  }
protected:
  Object *t_;
}; // class shared_base

/// \brief Scope-based auto-container for objects derived from Object class
///
/// Provides type-casting for further convenience.
/// \warning This is separated from shared_base due to code-size optimization
template <typename T> class shared : public shared_base {
public:
  inline shared(T *t = nullptr) : shared_base(t) {}
#if !defined(_MSC_VER)
  inline explicit operator bool() const { return t_; }
#endif
  inline operator T*() const { return t_; }
  inline operator const T*() const { return t_; }
  inline T *get() const { return static_cast<T*>(t_); }
  inline T &operator*() const { return *static_cast<T*>(t_); }
  inline T *operator->() const { return static_cast<T*>(t_); }
  inline shared &operator=(T *t) { reset(t); return *this; }
}; // class shared<>

typedef shared<Object> SAnything;
} // namespace kapusha
