#include "Filesystem_posix.h"
#include "File_posix.h"

namespace kapusha {
namespace io {

Filesystem_posix::Filesystem_posix(const char *root) : root_(root) {}

IFile *Filesystem_posix::open_file(const char *filename) const {
  return new File_posix(root_ + "/" + filename);
}
  
} // namespace io
} // namespace kapusha
