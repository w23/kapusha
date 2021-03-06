#define _POSIX_C_SOURCE 199309
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef KP_OVR
#include <OVR_CAPI.h>
#endif

#include <kapusha/sys.h>
#include "x11.h"

#define KP__SYS "startup"

static KPtime_ns time_offset = 0;

KPtime_ns kpSysTimeNs() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (ts.tv_sec * 1000000000ULL + ts.tv_nsec) - time_offset;
}

void kpSysAbort() {
  abort();
}

void kp__SysLogOutput(const char *output) {
  puts(output);
}

KPsize kpOutputsSelect(KPuptr *selectors, KPoutput_o *outputs, KPsize max) {
  return kp__X11OutputsSelect(selectors, outputs, max);
}

KPsize kpInputsSelect(KPuptr *selectors, KPinput_o *inputs, KPsize max) {
  return kp__X11InputsSelect(selectors, inputs, max);
}

int main(int argc, char *argv[])
{
  time_offset = kpSysTimeNs();
  kp__X11Init();
#ifdef KP_OVR
  ovr_Initialize();

  ovrHmd hmd = ovrHmd_Create(0);
  if (hmd) {
    KP__L("name: %s, manf: %s", hmd->ProductName, hmd->Manufacturer);
    KP__L("%d,%d %dx%d", hmd->WindowsPos.x, hmd->WindowsPos.y, hmd->Resolution.w, hmd->Resolution.h);
  }
#endif
  kpuserAppCreate(argc, (const char**)argv);
  kp__X11Run();
  kpuserAppDestroy();
  return 0;
}
