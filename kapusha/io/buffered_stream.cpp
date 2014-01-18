#include <string.h>
#include "buffered_stream.h"

namespace kapusha {
namespace io {
  
buffered_stream_t::error_e buffered_stream_t::set_error(error_e error) {
  error_ = error;
  return refill_zeroes(this);
}
  
buffered_stream_t::error_e buffered_stream_t::refill_zeroes(
  buffered_stream_t *stream) {
  static const u8 zero_buffer[256] = { 0 };
  stream->cursor_ = zero_buffer;
  stream->end_ = zero_buffer + sizeof(zero_buffer) / sizeof(*zero_buffer);
  stream->refill_ = refill_zeroes;
  return stream->error_;
}

buffered_stream_t::error_e buffered_stream_t::refill_zeroes_end(
  buffered_stream_t *stream) {
  stream->error_ = error_e::end;
  return refill_zeroes(stream);
}

  
} // namespace io
} // namespace kapusha