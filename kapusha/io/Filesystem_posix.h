#pragma once
#include <kapusha/core/String.h>
#include "IFilesystem.h"

namespace kapusha {
namespace io {

class Filesystem_posix : public IFilesystem {
public:
  Filesystem_posix(const char *root);
  IFile *open_file(const char *filename) const override;

private:
  core::string_t root_;
};

} // namespace io
} // namespace kapusha
