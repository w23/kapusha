#include <cstdlib>
#include "core.h"
namespace kapusha {
  void assert(const char *expr, const char *file, const char *line, const char *func) {
    L("%s:%s (%s): KP_ASSERT(%s) FAILED", file, line, func, expr);
    abort();
  }
} 
