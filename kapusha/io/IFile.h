#pragma once
#include <kapusha/core/Object.h>
#include "buffered_stream.h"

namespace kapusha {
namespace io {
  
class IFile : public core::Object {
public:
  struct info_t {
    size_t size;
  };
  inline const info_t &info() const { return info_; }

  struct file_stream_t : public buffered_stream_t {
    friend class IFile;
    inline file_stream_t(IFile &file) : file_(file) {}
    inline void set_refiller(buffered_stream_t::refill_f f) { refill_ = f; }
    IFile &file_;
  };
  inline buffered_stream_t &stream() { return stream_; }
  inline const buffered_stream_t &stream() const { return stream_; }
  
  virtual buffered_stream_t::error_e seek(size_t absolute_position) = 0;
  
protected:
  inline IFile() : stream_(*this) {}
  info_t info_;
  file_stream_t stream_;
};
  
} // namespace io
} // namespace kapusha