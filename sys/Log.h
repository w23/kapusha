#pragma once

#include <cstdio>

namespace kapusha {
  class Log {
  public:
    class ISystemLog {
    public:
      virtual ~ISystemLog() {}
      virtual void write(const char*) = 0;
    };

  public:
    Log();
    ~Log();

    static void open(const char* filename, ISystemLog *syslog = 0);

    static void write(const char* format, ...);

  private:
    FILE *file_;
    ISystemLog *syslog_;
  };

} // namespace kapusha

#define SP_LOG_OPEN(f,i) ::kapusha::Log::open(f,i)
#define L(f,...) ::kapusha::Log::write(f, __VA_ARGS__)

//! \todo levels, subsystems, call-stack depth with files and lines (in debug mode)