#pragma once
#include "IFilesystem.h"

namespace kapusha {
namespace io {

class Filesystem_posix : public IFilesystem {
public:
  IFile *open_file(const char *filename) const override;
};

} // namespace io
} // namespace kapusha