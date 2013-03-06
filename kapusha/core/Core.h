#pragma once

#include <cassert>

#include "types.h"
#include "Log.h"

#if DEBUG
#define KP_ASSERT(a) assert((a))
#define KP_ENSURE(a) assert((a))
#else
#define KP_ASSERT(a) {}
#define KP_ENSURE(a) (void)(a)
#endif

//! \todo custom assert to be able to dump stack trace to log file
