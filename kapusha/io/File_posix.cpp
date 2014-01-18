#include <fcntl.h> // ::open
#include <sys/stat.h> // ::stat
#include <sys/mman.h> // ::mmap
#include <unistd.h> // ::close
#include <kapusha/core/log.h>
#include "File_posix.h"

namespace kapusha {
namespace io {
  
buffered_stream_t::error_e File_posix::file_mmap_refill(
  buffered_stream_t *stream) {
  IFile::file_stream_t *fs = static_cast<IFile::file_stream_t*>(stream);
  File_posix &f = static_cast<File_posix&>(fs->file_);
  stream->cursor_ = reinterpret_cast<const u8*>(f.mapping());
  stream->end_ = stream->cursor_ + f.info().size;
  stream->refill_ = buffered_stream_t::refill_zeroes_end;
  return stream->error_;
}

File_posix::File_posix(const char *filename) {
  file_ = ::open(filename, O_RDONLY);
  if (file_ == -1) {
    L("Cannot open file %s", filename);
    stream_.set_error(buffered_stream_t::error_e::corrupted);
    return;
  }
  
  struct ::stat st;
  ::fstat(file_, &st);
  mapping_ = ::mmap(0, static_cast<size_t>(st.st_size),
    PROT_READ, MAP_PRIVATE, file_, 0);
  if (!mapping_) {
    L("Cannot mmap file %s of size %ull", filename, st.st_size);
    stream_.set_error(buffered_stream_t::error_e::corrupted);
    return;
  }
}
  
File_posix::~File_posix() {
  ::munmap(mapping_, size_);
  ::close(file_);
}
  
buffered_stream_t::error_e File_posix::seek(size_t absolute_position) {
  KP_ASSERT(!"NOT IMPLEMENTED");
  return stream_.set_error(buffered_stream_t::error_e::truncated);
}

} // namespace io
} // namespace kapusha