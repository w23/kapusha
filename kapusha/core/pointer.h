#pragma once

namespace kapusha {
/// \brief Smart pointer that retains ownership of an object
///
/// Basically, this is a dumbed-down analog of std::unique_ptr
template <typename T>
class pointer {
public:
  inline pointer(T *ptr = nullptr) : ptr_(ptr) {}
  inline pointer(pointer &&other) : ptr_(other.ptr_) { other.ptr_ = nullptr; }
  ~pointer() { delete ptr_; }

  inline pointer &operator=(T *ptr) {
    delete ptr_;
    ptr_ = ptr;
    return *this;
  }
  
  pointer &operator=(pointer &&other) {
    if (&other == this) return *this;
    delete ptr_;
    ptr_ = other.ptr_;
    other.ptr_ = nullptr;
    return *this;
  }

  operator bool() const { return nullptr != ptr_; }
  operator T*() const { return ptr_; }
  T &operator*() const { return *ptr_; }
  T *operator->() const { return ptr_; }
  
  inline T *release() {
    T *ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }

private: // disable copying
  pointer(const pointer &other);
  pointer &operator=(const pointer &other);

private:
  T *ptr_;
};
} // namespace kapusha