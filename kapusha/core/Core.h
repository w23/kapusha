#pragma once

#include <cassert>

#include "Log.h"

#if DEBUG
#define KP_ASSERT(a) assert((a))
#define KP_ENSURE(a) assert((a))
#else
#define KP_ASSERT(a) {}
#define KP_ENSURE(a) (void)(a)
#endif

//! \todo custom assert to be able to dump stack trace to log file

namespace kapusha {

  typedef unsigned char u8;
  typedef signed char s8;
  typedef unsigned short u16;
  typedef signed short s16;
  typedef unsigned int u32;
  typedef signed int s32;
  typedef unsigned long long u64;
  typedef signed long long s64;

}
