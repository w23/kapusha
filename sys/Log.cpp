#include <cstdarg>
#include "System.h"
#include "Log.h"

namespace kapusha {

#define LOG_MESSAGE_BUFFER_SIZE 4096

  static Log g_log;
  static char g_log_messsage_buffer[LOG_MESSAGE_BUFFER_SIZE];

  Log::Log() : file_(0), syslog_(0) {}
  Log::~Log()
  {
    if (file_)
      fclose(file_);

    if (syslog_)
      delete syslog_;
  }

  void Log::open(const char* filename, ISystemLog *syslog)
  {
    KP_ASSERT(!g_log.file_);
    KP_ASSERT(!g_log.syslog_);
    g_log.syslog_ = syslog;
    if (filename)
    {
      g_log.file_ = fopen(filename, "a");
      KP_ASSERT(g_log.file_);
    }
  }

  void Log::write(const char* format, ...)
  {
    va_list args;
    va_start(args, format);
    vsnprintf(g_log_messsage_buffer, LOG_MESSAGE_BUFFER_SIZE, format, args);
    
    if (g_log.file_)
    {
      //! \todo timestamp
      fprintf(g_log.file_, "%s\n", g_log_messsage_buffer);
    }

    if (g_log.syslog_)
      g_log.syslog_->write(g_log_messsage_buffer);
    
    va_end(args);
  }

} // namespace kapusha