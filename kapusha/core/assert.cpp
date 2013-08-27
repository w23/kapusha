#include <cstdlib>
#include "core.h"
namespace kapusha {
  void __assert_perror(const char *expr, const char *file, int line, const char *func) {
    L("%s:%d (%s): KP_ASSERT(%s) FAILED", file, line, func, expr);
    abort();
  }
} 
