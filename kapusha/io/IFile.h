#pragma once
#include <kapusha/core/Object.h>
#include "pipe.h"

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

  /// \brief Read some bytes from file into the pipe
  ///
  /// \warning This method is completely synchronous, meaning that it doesn't
  /// return until all data is piped into the pipe, or an error occures.
  /// \warning Method provides no means to error handling, you should ask
  /// your pipe whether it's ok or not
  /// \param pipe The pipe where to pipe the data
  /// \param offset Offset from the file beginning, in bytes
  /// \param size Number of bytes to read, zero means the whole file remainder
  /// \returns Number of bytes actually read
  virtual size_t pipe(pipe_t *pipe, size_t offset, size_t size = 0) = 0;

  inline operator bool() const { return valid_; }

protected:
  inline IFile() : valid_(false) {}
  bool valid_;
  info_t info_;
};
  
} // namespace io
} // namespace kapusha