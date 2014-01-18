#include "Filesystem_posix.h"
#include "File_posix.h"

namespace kapusha {
namespace io {

IFile *Filesystem_posix::open_file(const char *filename) const {
  return new File_posix(filename);
}
  
} // namespace io
} // namespace kapusha