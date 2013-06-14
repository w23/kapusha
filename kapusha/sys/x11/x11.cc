#include "extern.h"
#include "GLXViewportController.h"

namespace kapusha {
void log::sys_write(const char *message) {
  fprintf(stderr, "%s\n", message);
}

int X11Run(IViewport *viewport, vec2i size, bool fullscreen) {
  sys::GLXViewportController controller;
  return controller.run(viewport, size, fullscreen);
}

} // namespace kapusha
