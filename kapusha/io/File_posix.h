#pragma once
#include "IFile.h"

namespace kapusha {
namespace io {
  
class File_posix : public IFile {
public:
  File_posix(const char *filename);
  ~File_posix();
  virtual size_t pipe(pipe_t *pipe, size_t offset, size_t size) override;
private:
  int file_;
  void *mapping_;
}; // class File_posix
  
} // namespace io
} // namespace kapusha