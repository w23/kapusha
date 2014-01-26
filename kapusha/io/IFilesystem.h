#pragma once
#include <kapusha/core/Object.h>

namespace kapusha {
namespace io {

class IFile;
class IFilesystem : public core::Object {
public:
  virtual IFile *open_file(const char *filename) const = 0;
  static IFilesystem *create_native();
};
  
} // namespace io
} // namespace kapusha