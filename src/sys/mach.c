#include <mach/mach_time.h>
#include <stdlib.h>
#include "kapusha/core.h"

static mach_timebase_info_data_t timebase;
static uint64_t timeoffset;

void kp__mach_init() {
  mach_timebase_info(&timebase);
  timeoffset = mach_absolute_time();
}

KPtime_ns kpSysTimeNs() {
  uint64_t now = mach_absolute_time() - timeoffset;
  return now * timebase.numer / timebase.denom;
}

void kpSysAbort() {
  abort();
}