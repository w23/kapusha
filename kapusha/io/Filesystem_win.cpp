#include "Filesystem_win.h"
#include "File_win.h"

namespace kapusha {
namespace io {

IFile *Filesystem_win::open_file(const char *filename) const {
  return new File_win(filename);
}
  
} // namespace io
} // namespace kapusha