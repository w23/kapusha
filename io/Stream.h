#pragma once

#include <algorithm>
#include "../sys/System.h"

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
    typedef Error (RefillFunc)(Stream*);

  public:
    virtual ~Stream() {}
    unsigned size() const {
      return end_ - cursor_;
    }
    Error refill() { 
      return refill_func_(this);
    }

    const void* read(unsigned size) {
      SP_ASSERT(size <= this->size());
      const void* ret = cursor_;
      cursor_ += size;
      return ret;
    }

    template <typename F>
    Error process(int size)
    {
      do {
      unsigned chunk = (bytes > size()) ? size() : bytes;
      if (!chunk)
      {
        if (refill() != ErrorNone)
          return error_;
        continue;
      }
      F(cursor_, chunk);
      cursor_ += chunk;
      to = ((u8*)to + chunk);
      bytes -= chunk;
    } while (bytes > 0);
    return error_;
    }

    Error copy(void* to, unsigned bytes);

  public:
    const char *start_;
    const char *cursor_;
    const char *end_;
    Error error_;
    RefillFunc* refill_func_;

    static Error fail(Stream* stream, Error error);
    static Error refillZeroes(Stream* stream);
    static Error refillZeroesAtEnd(Stream* stream);
  };

//////////////////////////////////////////////////////////////////////////////

  class StreamSeekable : public Stream {
  public:
    enum Reference {
      ReferenceStart,
      ReferenceCursor,
      ReferenceEnd
    };
    typedef Error (SeekFunc)(StreamSeekable*, int offset, Reference ref);

    Error seek(int offset, Reference ref = ReferenceCursor) {
      return seek_func_(this, offset, ref);
    }

  public:
    SeekFunc *seek_func_;
  };

  //! \todo class StreamMemory : public StreamSeekable

} // namespace kapusha