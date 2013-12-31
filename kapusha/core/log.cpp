#include <cstdarg>
#include <cstdio>
#include "core.h"
#include "limits.h"

namespace kapusha {
namespace core {
  static FILE *g_log_file;
  static char g_log_messsage_buffer[KAPUSHA_MAX_LOG_MESSAGE_SIZE];
  void log::open(const char* filename) {
    KP_ASSERT(!g_log_file);
    if (filename) {
      g_log_file = fopen(filename, "a");
      KP_ASSERT(g_log_file);
    }
  }

  void log::write(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(g_log_messsage_buffer, KAPUSHA_MAX_LOG_MESSAGE_SIZE, format, args);
    if (g_log_file) {
      //! \todo timestamp
      fprintf(g_log_file, "%s\n", g_log_messsage_buffer);
    }
    sys_write(g_log_messsage_buffer);
    va_end(args);
  }
} // namespace core
} // namespace kapusha
