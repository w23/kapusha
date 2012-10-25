#include "Stream.h"

namespace kapusha {

#define STREAM_ZERO_BUFFER_SIZE 256
  static const char g_StreamZeroes[STREAM_ZERO_BUFFER_SIZE] = { 0 };

  Stream::Error Stream::fail(Stream* stream, Error error)
  {
    refillZeroes(stream);
    return stream->error_ = error;
  }

  Stream::Error Stream::refillZeroes(Stream* stream)
  {
    stream->start_ = stream->cursor_ = g_StreamZeroes;
    stream->end_ = g_StreamZeroes + STREAM_ZERO_BUFFER_SIZE;
    stream->refill_func_ = refillZeroes;
    return stream->error_;
  }

  Stream::Error Stream::refillZeroesAtEnd(Stream* stream)
  {
    stream->refill_func_ = refillZeroes;
    return stream->error_ = Error::ErrorEnd;
  }

  Stream::Error Stream::copy(void* to, unsigned bytes)
  {
    do {
      unsigned chunk = (bytes > size()) ? size() : bytes;
      if (!chunk)
      {
        if (refill() != ErrorNone)
          return error_;
        continue;
      }
      memcpy(to, cursor_, chunk);
      cursor_ += chunk;
      to = ((u8*)to + chunk);
      bytes -= chunk;
    } while (bytes > 0);
    return error_;
  }

} // namespace kapusha