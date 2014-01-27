#if KP_OS_WINDOWS
#include "Filesystem_win.h"
#elif KP_OS_POSIX
#include "Filesystem_posix.h"
#endif

namespace kapusha {
namespace io {

IFilesystem *IFilesystem::create_native(const char *at_root) {
#if KP_OS_WINDOWS
  return new Filesystem_win(at_root);
#elif KP_OS_POSIX
  return new Filesystem_posix(at_root);
#endif
}

} // namespace io
} // namespace kapusha