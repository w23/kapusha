#include "../sys/Log.h"
#include "StreamFile.h"

namespace kapusha {

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
    file_ = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, 
                        OPEN_EXISTING, 0, NULL);
    if (file_ == INVALID_HANDLE_VALUE)
    {
      L("Error opening file \"%s\"", filename);
      fail(this, ErrorCorrupted);
      return error_;
    }

    mapping_ = CreateFileMapping(file_, NULL, PAGE_READONLY, 0, 0, NULL);
    void* ptr;
    if (mapping_ == INVALID_HANDLE_VALUE || 
        !(ptr = MapViewOfFile(mapping_, FILE_MAP_READ, 0, 0, 0)))
    {
      L("Error mmapping file \"%s\"", filename);
      close();
      fail(this, ErrorTruncated);
      return error_;
    }

    size_t size = GetFileSize(file_, NULL);
    useMemory(ptr, size);

    L("Opened file \"%s\" stream @%p size %ld", filename, ptr, size);

    return error_ = ErrorNone;
  }

} // namespace kapusha