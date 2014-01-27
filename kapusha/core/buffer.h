#pragma once
#include <cstring> // memcpy
#include "types.h"
#include "assert.h"

namespace kapusha {
namespace core {

/// \brief simple buffer for \a stuff
struct buffer_t {
  explicit buffer_t(size_t size = 0, const void *data = nullptr);
  explicit buffer_t(const buffer_t &other);
  explicit buffer_t(buffer_t &&other);
  ~buffer_t();

  inline bool empty() const { return size_ == 0; }
  inline size_t size() const { return size_; }
  inline const char *data() const { return data_; }
  inline char *data() { return data_; }

  template <typename T> inline const T *data_as() const {
    return reinterpret_cast<const T*>(data_);
  }
  template <typename T> inline T *data_as() {
    return reinterpret_cast<T*>(data_);
  }

  buffer_t &operator=(const buffer_t &other);
  buffer_t &operator=(buffer_t &&other);
  bool operator==(const buffer_t &other) const;

  /// \warning invalidates all previously given pointers
  void resize(size_t new_size);

  inline void copy(size_t offset, const void *src, size_t size) {
    KP_ASSERT(offset + size < size_);
    copy(data_ + offset, src, size);
  }
  
  /// \todo safe_copy that does resize() if needed

  static inline void copy(void *dest, const void *src, size_t size) {
    ::memcpy(dest, src, size);
  }

private:
  size_t size_;
  char *data_;

  void init_copy(size_t size, const void *data);
}; // struct buffer_t

} // namespace core
} // namespace kapusha
