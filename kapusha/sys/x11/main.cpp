#include "../sys.h"
#include "GLXViewportController.h"

using namespace kapusha;

namespace kapusha {
void core::log::sys_write(const char *message) {
  fprintf(stderr, "KPLOG: %s\n", message);
}
} // namespace kapusha

int main(int argc, char *argv[]) {
  sys::GLXViewportController controller;
  return controller.run(kapusha_main(nullptr));
}
