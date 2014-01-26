#if KP_OS_WINDOWS
#include "Filesystem_win.h"
#elif KP_OS_POSIX
#include "Filesystem_posix.h"
#endif

namespace kapusha {
namespace io {

IFilesystem *IFilesystem::create_native() {
  return new Filesystem_posix();
}

} // namespace io
} // namespace kapusha