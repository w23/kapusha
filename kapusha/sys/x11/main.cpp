#include "../sys.h"
#include "GLXViewportController.h"

namespace kapusha {
void core::log::sys_write(const char *message) {
  fprintf(stderr, "KPLOG: %s\n", message);
}
} // namespace kapusha

using namespace kapusha;

int main(int argc, char *argv[]) {
  core::StringArray::shared args(new core::StringArray(argc));
  for (int i = 0; i < argc; ++i)
    args->push_back(new core::String(argv[i]));

  sys::GLXViewportController controller;
  return controller.run(kapusha_main(args));
}
