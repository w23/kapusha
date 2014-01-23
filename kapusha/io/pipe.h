#pragma once
#include <kapusha/core/types.h>

namespace kapusha {
namespace io {

/// \brief Basic interface for stuffs that want to receive some bytes
///
/// \warning I have no idea of what I have done here.
struct pipe_t {
  /// \brief Response description
  struct response_t {
    /// \brief Number of bytes that were consumed
    /// Orders the producer to shift it's data cursor by \p consumed bytes
    size_t consumed;

    /// \brief What producer should do
    ///
    /// Error handling is deliberately not addressed here
    enum class order_e {
      /// Resend data again, while still respecting \p consumed value
      ///
      /// This might seem pointless at first, but pipe might update it's
      /// internal state (and/or also change the pipe function) and simple
      /// repeat would be more convenient than buffer management shit
      repeat,

      /// Give more data. MORE
      more,

      /// Enough is enough, stop piping this shit to me.
      /// This does not mean an error.
      enough
    } order;

    inline response_t(size_t cons, order_e ord)
      : consumed(cons), order(ord) {}
  }; // struct response_t

  /// \brief Pipe function prototype
  ///
  /// This functions gets called when new data arrives.
  /// \param pipe Basically, this.
  /// \param data Pointer to the data
  /// \param size Size of the data in bytes
  /// \returns Response that says how much data was read and what to do next
  typedef response_t (*pipe_f)(pipe_t *pipe, const void *data, size_t size);

  /// \brief Pointer to the function where all the data gets piped into
  ///
  /// \remark This pointer may change during runtime wherever it feels like it
  pipe_f pipe;
}; // struct pipe_t

/// \brief A very nice function that will pipe your data. SALE 90% OFF
///
/// \warning Will do it completely synchronously
/// \param pipe Where to pipe
/// \param data All your valuables
/// \param bytes How many
/// \returns How many bytes were accepted
pipe_t::response_t pipe_memory(pipe_t *pipe, const void *data, size_t bytes);

} // namespace io
} // namespace kapusha
