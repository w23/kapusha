#pragma once
#include <kapusha/core/types.h>
#include <kapusha/core/assert.h>

namespace kapusha {
namespace io {

struct buffered_stream_t {
  enum class error_e { none, end, truncated, corrupted };
  typedef error_e (*refill_f)(buffered_stream_t *stream);
  typedef void (*dtor_f)(buffered_stream_t *stream);

  inline buffered_stream_t() : cursor_(nullptr), end_(nullptr)
    , error_(error_e::none), dtor_(nullptr) {}
  inline ~buffered_stream_t() { if (dtor_) dtor_(this); }

  inline bool empty() const { return cursor_ == end_; }
  inline size_t size() const { return end_ - cursor_; }
  inline const void *cursor() const { return cursor_; }
  inline const void *read(size_t size) {
    KP_ASSERT(this->size() >= size);
    const u8 *ret = cursor_;
    cursor_ += size;
    return ret;
  }
  inline error_e refill() { return refill_(this); }

  error_e set_error(error_e error);

public:
  const u8 *cursor_, *end_;
  error_e error_;
  refill_f refill_;
  dtor_f dtor_;
  
  static error_e refill_zeroes(buffered_stream_t *stream);
  static error_e refill_zeroes_end(buffered_stream_t *stream);
}; // struct stream_t

} // namespace io
} // namespace kapusha
