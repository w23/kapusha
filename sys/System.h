#pragma once

#include <cassert>

#if DEBUG
#define KP_ASSERT(a) assert((a))
#define KP_ENSURE(a) assert((a))
#else
#define KP_ASSERT(a) {}
#define KP_ENSURE(a) a
#endif

//! \todo custom assert to be able to dump stack trace to log file

namespace kapusha {

  typedef unsigned char u8;
  typedef signed char s8;
  typedef unsigned short u16;
  typedef unsigned int u32;
  typedef unsigned long long u64;

}
