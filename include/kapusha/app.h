#pragma once

#include <kapusha/core.h>

#ifdef __cplusplus
extern "C" {
#endif

/* should be implemented by api user */
/* Entry point to dedicated application thread (not main()) */
int kpuserAppRun(int argc, const char **argv);

void kpAppDestroy(int exit_code /* = 0 for success */);

typedef struct KPcolorspace_t {
  KPf32 red_x, red_y;
  KPf32 green_x, green_y;
  KPf32 blue_x, blue_y;
  KPf32 white_x, white_y;
  KPf32 gamma;
} KPcolorspace_t;

enum {
  KPDisplayCombined = 0x0001,
  KPDisplayPrimary = 0x0002,
  KPDisplayOculus = 0x0003,
  KPDisplayActive = 0x0004
} KPDisplayFlags;

typedef struct KPdisplay_t { KP_O;
  int width, height;
  int width_mm, height_mm;
  KPf32 vppmm, hppmm;
  KPtime_ns frame_delta;
  KPu32 flags;
  KPcolorspace_t colorspace;
} KPdisplay_t, *KPdisplay_o;

KPsize kpAppDisplaySelect(KPuptr *selectors, KPdisplay_o *dpys, KPsize max);

#ifdef __cplusplus
} // extern "C"
#endif
