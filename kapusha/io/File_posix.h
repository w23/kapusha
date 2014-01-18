#pragma once
#include "IFile.h"

namespace kapusha {
namespace io {
  
class File_posix : public IFile {
public:
  File_posix(const char *filename);
  ~File_posix() override;
  buffered_stream_t::error_e seek(size_t absolute_position) override;
  
private:
  int file_;
  void *mapping_;
  size_t size_;
  
  static buffered_stream_t::error_e file_mmap_refill(buffered_stream_t *stream);
  inline const void *mapping() const { return mapping_; }
}; // class File_posix
  
} // namespace io
} // namespace kapusha