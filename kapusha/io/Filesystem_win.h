#pragma once
#include <kapusha/core/String.h>
#include "IFilesystem.h"

namespace kapusha {
namespace io {

class Filesystem_win : public IFilesystem {
public:
  Filesystem_win(const char *at_root);
  IFile *open_file(const char *filename) const override;

private:
  core::string_t root_;
};

} // namespace io
} // namespace kapusha