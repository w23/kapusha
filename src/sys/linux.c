#define _POSIX_C_SOURCE 199309
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <kapusha/sys.h>
#include "x11.h"

KPtime_ns kpSysTimeNs() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

void kpSysExit(int code) {
  KP_UNUSED(code);
  abort();
}

void kp__LogOutput(const char *output) {
  puts(output);
}

int main(int argc, char *argv[])
{
  kp__X11Init();
  appConfigure(argc, (const char**)argv);
  kp__X11Run();
  return 0;
}
