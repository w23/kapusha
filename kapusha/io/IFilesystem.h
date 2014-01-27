#pragma once
#include <kapusha/core/shared.h>

namespace kapusha {
namespace io {

class IFile;
class IFilesystem : public core::Object {
public:
  typedef core::shared<IFilesystem> shared;
  virtual IFile *open_file(const char *filename) const = 0;
  static IFilesystem *create_native(const char *at_root = nullptr);
};
  
} // namespace io
} // namespace kapusha