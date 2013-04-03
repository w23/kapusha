#pragma once
namespace kapusha {
  void assert_perror(const char *expr, const char *file, const char *line, const char *func);
}

#if DEBUG
#define KP_ASSERT(expr) ((expr) ? 0 : ::kapusha::assert(#expr, __FILE__, __LINE__, __PRETTY_FUNCTION__))
#define KP_ENSURE(expr) KP_ASSERT(expr)
#else
#define KP_ASSERT(expr) {}
#define KP_ENSURE(expr) (void)(expr)
#endif
