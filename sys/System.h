#pragma once

#include <cassert>

#if DEBUG
#define SP_ASSERT(a) assert((a))
#else
#define SP_ASSERT(a) {}
#endif

//! \todo custom assert to be able to dump stack trace to log file

namespace kapusha {

  typedef unsigned char u8;
  typedef unsigned short u16;
  typedef unsigned int u32;
  typedef unsigned long long u64;

}