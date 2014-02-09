#include "Filesystem_posix.h"
#include "File_posix.h"

namespace kapusha {
namespace io {

Filesystem_posix::Filesystem_posix(const char *root) : root_(root?root:"") {}

IFile *Filesystem_posix::open_file(const char *filename) const {
  return new File_posix(root_.empty() ? filename : root_ + "/" + filename);
}

IFilesystem *IFilesystem::create_native(const char *at_root) {
  return new Filesystem_posix(at_root);
}
  
} // namespace io
} // namespace kapusha
