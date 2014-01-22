#pragma once
#include "IFilesystem.h"

namespace kapusha {
namespace io {

class Filesystem_win : public IFilesystem {
public:
  IFile *open_file(const char *filename) const override;
};

} // namespace io
} // namespace kapusha