#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Kill current process */
void kpSysExit(int code);

/* Get current precise monotonic system time in nanoseconds */
KPtime_ns kpSysTimeNs();

#ifdef __cplusplus
} // extern "C"
#endif
