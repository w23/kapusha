#include <string.h>
#include "Stream.h"

namespace kapusha {

#define STREAM_ZERO_BUFFER_SIZE 256
  static const char g_StreamZeroes[STREAM_ZERO_BUFFER_SIZE] = { 0 };

  void Stream::fail(Stream* stream, Error error)
  {
    refillZeroes(stream);
    stream->error_ = error;
  }

  void Stream::refillZeroes(Stream* stream)
  {
    stream->start_ = stream->cursor_ = g_StreamZeroes;
    stream->end_ = g_StreamZeroes + STREAM_ZERO_BUFFER_SIZE;
    stream->refill_func_ = refillZeroes;
  }

  void Stream::refillZeroesAtEnd(Stream* stream)
  {
    stream->refill_func_ = refillZeroes;
    stream->error_ = ErrorEnd;
  }

  Stream::Error Stream::copy(void* to, size_t bytes)
  {
    do {
      size_t chunk = (bytes > size()) ? size() : bytes;
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
  
////////////////////////////////////////////////////////////////////////////////
  
  static void seekInMemory(Stream* stream, off_t offset,
                           Stream::Reference ref)
  {
    StreamMemory* memory = static_cast<StreamMemory*>(stream);
    
    // translate offset to absolute
    switch (ref)
    {
      case Stream::ReferenceEnd:
        offset += memory->size_;
      case Stream::ReferenceStart:
        break;
      case Stream::ReferenceCursor:
        // check whether we're inside the valid memory now
        if (memory->start_ == memory->pointer_)
          offset += memory->cursor_ - memory->start_;
        else // otherwise don't seek
          return;
        break;
    }
    
    // if absolute offset is valid
    if (offset >= 0 && offset < static_cast<off_t>(memory->size_))
    {
      // if setup is wrong, redo it
      if (memory->start_ != memory->pointer_)
      {
        memory->start_ = memory->pointer_;
        memory->end_ = memory->start_ + memory->size_;
        memory->error_ = Stream::ErrorNone;
        memory->refill_func_ = Stream::refillZeroesAtEnd;
      }
      memory->cursor_ = memory->start_ + offset;
    } else {
      memory->error_ = Stream::ErrorEnd;
      memory->refill_func_ = Stream::refillZeroes;
      memory->refill();
    }
  }
  
  void StreamMemory::useMemory(const void *ptr, size_t size)
  {
    pointer_ = static_cast<const char*>(ptr);
    size_ = size;
    seek_func_ = seekInMemory;
    seek_func_(this, 0, ReferenceStart);
  }

} // namespace kapusha
