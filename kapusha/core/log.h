#pragma once
namespace kapusha {
  class log {
  public:
    static void open(const char *filename);
    static void write(const char *format, ...);
    static void sys_write(const char *message);
  };
} // namespace kapusha

#define KP_LOG_OPEN(filename) ::kapusha::log::open(filename)
#define L(...) ::kapusha::log::write(__VA_ARGS__)

//! \todo levels, subsystems, call-stack depth with files and lines (in debug mode)
