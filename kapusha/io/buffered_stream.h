#pragma once
#include <kapusha/core/types.h>

namespace kapusha {
namespace io {

/// \brief Stream that owns data buffer
///
/// The stream owns the buffer with stream data.
/// Features (for consumer):
///  - Consumer may have no buffer at all.
///  - Always in readable state, even if theres an error. In this case it
///    produces an endless stream of zeroes.
///
/// Features (for producer):
///  - 
struct buffered_stream_t {
  enum class status_e { ok, end, error };
  
  inline const char *cursor() const { return cursor_; }
  inline const char *end() const { return end_; }
  inline size_t left() const { return end_ - cursor_; }
  inline status_e status() const { return status_; }

  /// \brief read next byte but don't move cursor
  inline char peek() const { return *cursor_; }

  /// \brief read next byte and advance cursor
  char read();

  /// \brief advance cursor
  status_e advance(size_t bytes);

  /// \brief More traditional "read bytes into my buffer" method
  status_e read(void *dst, size_t bytes);

  /// \brief Force production of more data.
  ///
  /// Invalidates data at current cursor position if left() > 0
  inline status_e produce() { produce_(this); return status_; }

  typedef void (*produce_f)(buffered_stream_t *stream);

  const char *cursor_, *end_;
  produce_f produce_;
  status_e status_;

  /// \brief (producer only) Produce stream of zeroes
  static void produce_zeroes(buffered_stream_t *stream);

  /// \brief (producer only) Produce stream of zeroes and set status_ to 'end'
  static void signal_end_and_produce_zeroes(buffered_stream_t *stream);

  /// \brief (producer only) Produce stream of zeroes and set status_ to 'error'
  static void signal_error_and_produce_zeroes(buffered_stream_t *stream);
}; // struct buffered_stream_t

/// \brief A way to stream memory region
struct memory_stream_t : public buffered_stream_t {
  memory_stream_t(const void *source, size_t size);
};

} // namespace io
} // namespace kapusha
