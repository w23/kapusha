#include <kapusha/core/wstring.h>
#include <kapusha/core/log.h>
#include "File_win.h"

namespace kapusha {
namespace io {

File_win::File_win(const char *filename) : file_(INVALID_HANDLE_VALUE)
, mapping_(INVALID_HANDLE_VALUE), mapping_ptr_(nullptr) {
  core::windows::wstring_t wilename(filename);
  file_ = ::CreateFileW(wilename, GENERIC_READ, FILE_SHARE_READ, NULL,
    OPEN_EXISTING, 0, NULL);
  if (file_ == INVALID_HANDLE_VALUE) {
    L("Error opening file \"%s\"", filename);
    return;
  }

  mapping_ = CreateFileMapping(file_, NULL, PAGE_READONLY, 0, 0, NULL);
  if (mapping_ == INVALID_HANDLE_VALUE ||
    !(mapping_ptr_ = ::MapViewOfFile(mapping_, FILE_MAP_READ, 0, 0, 0))) {
    L("Error mmapping file \"%s\"", filename);
    return;
  }

  info_.size = ::GetFileSize(file_, NULL);
  valid_ = true;
}

File_win::~File_win() {
  if (mapping_ptr_) ::UnmapViewOfFile(mapping_ptr_);
  if (mapping_ != INVALID_HANDLE_VALUE) ::CloseHandle(mapping_);
  if (file_ != INVALID_HANDLE_VALUE) ::CloseHandle(file_);
}

buffered_stream_t::status_e File_win::stream_chunk(
    buffered_stream_t &stream, size_t offset, size_t size) {
  const u8 *bytes = reinterpret_cast<const u8*>(mapping_ptr_);
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
