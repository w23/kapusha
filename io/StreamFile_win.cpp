#include "StreamFile.h"

//! \fixme this is very dirty and needs cleaning

namespace kapusha {

  static Stream::Error streamFileSeekFunc(StreamSeekable*, int offset, StreamSeekable::Reference ref);

  StreamFile::StreamFile(void)
    : file_(INVALID_HANDLE_VALUE)
    , mapping_(INVALID_HANDLE_VALUE)
  {
    fail(this, ErrorEnd);
  }

  StreamFile::~StreamFile(void)
  {
    close();
  }

  void StreamFile::close()
  {
    if (mapping_ != INVALID_HANDLE_VALUE)
    {
      UnmapViewOfFile(start_);
      CloseHandle(mapping_);
      mapping_ = INVALID_HANDLE_VALUE;
    }

    if (file_ != INVALID_HANDLE_VALUE)
    {
      CloseHandle(file_);
      file_ = INVALID_HANDLE_VALUE;
    }

    fail(this, ErrorEnd);
  }

  Stream::Error StreamFile::open(const char *filename)
  {
    file_ = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (file_ == INVALID_HANDLE_VALUE)
    {
      return fail(this, ErrorCorrupted);
    }

    mapping_ = CreateFileMapping(file_, NULL, PAGE_READONLY, 0, 0, NULL);
    if (mapping_ == INVALID_HANDLE_VALUE)
    {
      close();
      return fail(this, ErrorCorrupted);
    }

    start_ = static_cast<char*>(MapViewOfFile(mapping_, FILE_MAP_READ, 0, 0, 0));
    end_ = start_ + GetFileSize(file_, NULL);
    cursor_ = start_;

    //! \fixme this forbids us from seeking back
    refill_func_ = refillZeroesAtEnd;
    seek_func_ = streamFileSeekFunc;

    return error_ = ErrorNone;
  }

  Stream::Error StreamFile::streamFileSeekFunc(StreamSeekable *stream,
    int offset, Reference ref)
  {
    //! \fixme we could still seek back in most situations
    if (stream->error_ != Stream::ErrorNone)
      return stream->error_;

    StreamFile *file = static_cast<StreamFile*>(stream);

    //! \fixme range error checks
    switch (ref)
    {
    case ReferenceStart:
      file->cursor_ = file->start_ + offset;
      break;
    case ReferenceEnd:
      file->cursor_ = file->end_ + offset;
      break;
    case ReferenceCursor:
      file->cursor_ += offset;
      break;
    }

    return ErrorNone;
  }

} // namespace kapusha