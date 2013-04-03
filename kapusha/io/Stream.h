#pragma once
#include <sys/types.h>
#include "../core/core.h"

namespace kapusha {
  // hi, ryg!
  class Stream {
  public:
    enum Error {
      ErrorNone = 0,
      ErrorEnd,
      ErrorTruncated,
      ErrorCorrupted
    };
    enum Reference {
      ReferenceStart,
      ReferenceCursor,
      ReferenceEnd
    };
    typedef void (RefillFunc)(Stream*);
    typedef void (SeekFunc)(Stream*, off_t offset, Reference ref);

  public:
    inline Stream() : seek_func_(0) {}
    virtual ~Stream() {}
    inline size_t size() const {
      return end_ - cursor_;
    }
    inline Error refill() {
      refill_func_(this);
      return error_;
    }

    inline const void* read(unsigned size) {
      KP_ASSERT(size <= this->size());
      const void* ret = cursor_;
      cursor_ += size;
      return ret;
    }

    template <typename PROC>
    Error process(int bytes)
    {
      do {
      size_t chunk = (bytes > size()) ? size() : bytes;
      if (!chunk)
      {
        if (refill() != ErrorNone)
          return error_;
        continue;
      }
      PROC(cursor_, chunk);
      cursor_ += chunk;
      bytes -= chunk;
    } while (bytes > 0);
    return error_;
    }

    Error copy(void* to, size_t bytes);
    
    inline bool isSeekable() const { return seek_func_ != 0; }
    
    inline Error seek(off_t offset, Reference ref = ReferenceCursor) {
      seek_func_(this, offset, ref);
      return error_;
    }

  public:
    const char *start_;
    const char *cursor_;
    const char *end_;
    Error error_;
    RefillFunc *refill_func_;
    SeekFunc *seek_func_;

    static void fail(Stream* stream, Error error);
    static void refillZeroes(Stream* stream);
    static void refillZeroesAtEnd(Stream* stream);
  };

  
////////////////////////////////////////////////////////////////////////////////

  //! \fixme if seek < start, will read zeroes indefinitely
  //! \fixme also, when over-/underflow, cursor-relative seek won't work at all
  //! \fixme absolute seek will bring sanity back
  class StreamMemory : public Stream
  {
  public:
    void useMemory(const void* ptr, size_t size);
    
  public:
    const char *pointer_;
    size_t size_;
  };

} // namespace kapusha
