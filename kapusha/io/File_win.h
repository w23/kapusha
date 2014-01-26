#pragma once
#define WIN32_LEAN_AND_MEAN 1
#define NOMINMAX 1
#include <windows.h>
#include "IFile.h"

namespace kapusha {
namespace io {

class File_win : public IFile {
public:
  File_win(const char *filename);
  ~File_win();
  virtual buffered_stream_t::status_e stream_chunk(
    buffered_stream_t &stream, size_t offset, size_t size) override;

private:
  HANDLE file_;
  HANDLE mapping_;
  const void *mapping_ptr_;
}; // class File_win

} // namespace io
} // namespace kapusha
