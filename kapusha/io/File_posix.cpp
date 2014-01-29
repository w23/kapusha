#include <fcntl.h> // ::open
#include <sys/stat.h> // ::stat
#include <sys/mman.h> // ::mmap
#include <unistd.h> // ::close
#include <kapusha/core/log.h>
#include "File_posix.h"

namespace kapusha {
namespace io {

File_posix::File_posix(const char *filename) : mapping_(nullptr) {
  file_ = ::open(filename, O_RDONLY);
  if (file_ == -1) {
    L("Cannot open file %s", filename);
    return;
  }

  struct ::stat st;
  ::fstat(file_, &st);
  mapping_ = ::mmap(0, static_cast<size_t>(st.st_size),
    PROT_READ, MAP_PRIVATE, file_, 0);
  if (!mapping_) {
    L("Cannot mmap file %s of size %ull", filename, st.st_size);
    return;
  }

  info_.size = static_cast<size_t>(st.st_size);
  valid_ = true;
}
  
File_posix::~File_posix() {
  if (mapping_) ::munmap(mapping_, info_.size);
  if (file_ != -1) ::close(file_);
}

buffered_stream_t::status_e File_posix::stream_chunk(
    buffered_stream_t &stream, size_t offset, size_t size) {
  const u8 *bytes = reinterpret_cast<const u8*>(mapping_);
  if (offset > info_.size) {
    buffered_stream_t::signal_end_and_produce_zeroes(&stream);
  } else {
    if (offset + size > info_.size) size = info_.size - offset;
    stream = memory_stream_t(bytes + offset, size);
  }
  return stream.status();
}


} // namespace io
} // namespace kapusha
