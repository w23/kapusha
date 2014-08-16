#define _POSIX_C_SOURCE 199309
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "kapusha/sys.h"

KPtime_ms KP_now_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL;
}

void kpSysExit(int code) {
  KP_UNUSED(code);
  abort();
}

void kp__LogOutput(const char *output) {
  puts(output);
}
