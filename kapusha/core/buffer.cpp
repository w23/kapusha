#include <cstring>
#include "buffer.h"

namespace kapusha {
buffer_t::buffer_t(size_t size, const void *data) : data_(nullptr) {
  init_copy(size, data);
}

void buffer_t::init_copy(size_t size, const void *data) {
  delete[] data_;
  size_ = size;
  data_ = nullptr;
  if (size_) {
    data_ = new char[size_];
    if (data) memcpy(data_, data, size_);
  }
}

buffer_t::buffer_t(const buffer_t &other) : data_(nullptr) {
  init_copy(other.size_, other.data_);
}

buffer_t::buffer_t(buffer_t &&other)
  : size_(other.size_), data_(other.data_) {
  other.size_ = 0;
  other.data_ = nullptr;
}

buffer_t::~buffer_t() {
  delete[] data_;
}

buffer_t &buffer_t::operator=(const buffer_t &other) {
  if (&other != this) init_copy(other.size_, other.data_);
  return *this;
}

buffer_t &buffer_t::operator=(buffer_t &&other) {
  if (&other != this) {
    delete[] data_;
    size_ = other.size_;
    data_ = other.data_;
    other.size_ = 0;
    other.data_ = nullptr;
  }
  return *this;
}

bool buffer_t::operator==(const buffer_t &other) const {
  if (size_ != other.size_) return false;
  return 0 == memcmp(data_, other.data_, size_);
}

} // namespace kapusha