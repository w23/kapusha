#include <kapusha/core/assert.h>
#include "pipe.h"

namespace kapusha {
namespace io {

size_t pipe_memory(pipe_t *pipe, const void *data, size_t bytes) {
  const u8 *p = static_cast<const u8*>(data);
  size_t left = bytes;
  while (left) {
    const pipe_t::response_t resp = pipe->pipe(pipe, p, left);
    KP_ASSERT(resp.consumed <= left);
    p += resp.consumed;
    left -= resp.consumed;
    switch (resp.order) {
    case pipe_t::response_t::order_e::more: // no more data will be available
    case pipe_t::response_t::order_e::enough:
      return bytes - left;
    case pipe_t::response_t::order_e::repeat:
      break;
    }
  }
  return bytes;
}

} // namespace io
} // namespace kapusha