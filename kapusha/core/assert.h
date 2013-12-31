#pragma once

/// \todo elaborate on this

namespace kapusha {
  void assert_perror(const char *expr, const char *file, int line, const char *func);
}

#if DEBUG
#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif
#define KP_ASSERT(expr) if(!(expr)) ::kapusha::assert_perror(#expr, __FILE__, __LINE__, __PRETTY_FUNCTION__)
#define KP_ENSURE(expr) KP_ASSERT(expr)
#else
#define KP_ASSERT(expr) {}
#define KP_ENSURE(expr) (void)(expr)
#endif
