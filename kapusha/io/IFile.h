#pragma once
#include <kapusha/core/Object.h>
#include "buffered_stream.h"

namespace kapusha {
namespace io {

/// \brief Basic file access
///
/// Acquired using IFilesystem object
class IFile : public core::Object {
public:
  /// \brief Metainformation
  struct info_t {
    /// \brief File size in bytes
    size_t size;
  };

  /// \brief Get file info
  ///
  /// \returns File information
  inline const info_t &info() const { return info_; }

  inline operator bool() const { return valid_; }

  virtual buffered_stream_t::status_e stream_chunk(
    buffered_stream_t &stream, size_t offset, size_t size) = 0;

protected:
  inline IFile() : valid_(false) {}
  bool valid_;
  info_t info_;
};
  
} // namespace io
} // namespace kapusha