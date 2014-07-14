#include <mach/mach_time.h>
#include "kapusha/sys.h"

static mach_timebase_info_data_t timebase;

void kp__mach_init() {
  mach_timebase_info(&timebase);
}

KPtime_ns kpSysTimeNs() {
  uint64_t now = mach_absolute_time();
  return now * timebase.numer / timebase.denom;
}
