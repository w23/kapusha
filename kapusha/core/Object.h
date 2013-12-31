#pragma once
#include <atomic>

/// \warning kapusha does not support multiple inheritance by design

namespace kapusha {
namespace core {

/// \brief Base object, provides reference-counting
///
/// Every non-trivial class instances of which are potentially to be shared
/// are to be derived from this
class Object {
public:
  /// Initial retain count should be zero
  /// Motivation: 1) most usages are more convenient with this
  ///             2) other usages mostly ok with manual delete
  Object() : retain_count_(0) {}

  /// Obviously every derived class should have virtual destructor
  virtual ~Object() {}

  /// Retain an object for further use by incrementing retain count
  inline void retain() { ++retain_count_; }

  /// Release an object by decrementing retain count.
  /// If retain count reaches zero the object is deleted
  void release();

  /// Get current retain count
  inline int retain_count() const { return retain_count_; }

private: // Object is noncopyable by default
  Object(const Object &object);
  Object &operator=(const Object &object);
  Object &operator=(const Object &&object);

private:
  std::atomic<int> retain_count_;
};

} // namespace core
} // namespace kapusha
