#include "Filesystem_win.h"
#include "File_win.h"

namespace kapusha {
namespace io {

Filesystem_win::Filesystem_win(const char *at_root) : root_(at_root) {}

IFile *Filesystem_win::open_file(const char *filename) const {
  return new File_win(root_ + "\\" + filename);
}
  
} // namespace io
} // namespace kapusha