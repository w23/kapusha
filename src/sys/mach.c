#include <mach/mach_time.h>
#include <stdlib.h>
#include "kapusha/core.h"

static mach_timebase_info_data_t timebase;

void kp__mach_init() {
  mach_timebase_info(&timebase);
}

KPtime_ns kpSysTimeNs() {
  uint64_t now = mach_absolute_time();
  return now * timebase.numer / timebase.denom;
}

static void *kp__DefaultAlloc(struct KPheap_header_t *heap, KPsize size) {
  return malloc(size);
}

void kp__DefaultFree(struct KPheap_header_t *heap, void *mem) {
  free(mem);
}

static KPheap_header_t kp__default_heap_header = {
  kp__DefaultAlloc, kp__DefaultFree };

KPheap_p kp__heap_default = &kp__default_heap_header;