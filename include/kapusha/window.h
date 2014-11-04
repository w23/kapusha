#pragma once
#include "kapusha/core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct KPoutput_t { KP_O;
  enum {
    KPOutputVideo
  } type;
} KPoutput_t, *KPoutput_o;

enum {
  KPVideoOutputCombined = 0x0001,
  KPVideoOutputPrimary = 0x0002,
  KPVideoOutputOculus = 0x0003
} KPVideoOutputFlags;

typedef struct KPoutput_video_t { KPoutput_t header;
  int width, height;
  int width_mm, height_mm;
  KPtime_ns frame_delta;
  KPu32 flags;
} KPoutput_video_t, *KPoutput_video_o;

enum {
  KPOutputSelectorType, /* KPOutputVideo, KPOutputAudio, default == any*/
  KPOutputSelectorFlags
} KPOutputSelectors;

KPiterable_o kpOutputsSelect(int *selectors);

typedef void *KPwindow_o;

typedef struct {
  KPwindow_o window;
  void *user_data;
} KPwindow_painter_create_t;

typedef struct {
  KPwindow_o window;
  void *user_data;
  int width, height;
} KPwindow_painter_configure_t;

typedef struct {
  KPwindow_o window;
  void *user_data;
  KPtime_ns pts_estimate;
  KPtime_ns time_delta;
  KPtime_ns time_delta_frame;
} KPwindow_painter_t;

typedef void (*KPwindow_painter_create_f)(const KPwindow_painter_create_t *);
typedef void (*KPwindow_painter_configure_f)(const KPwindow_painter_configure_t *);
typedef void (*KPwindow_painter_f)(const KPwindow_painter_t *);

typedef struct {
  void *user_data;
  KPwindow_painter_create_f painter_create_func;
  KPwindow_painter_configure_f painter_configure_func;
  KPwindow_painter_f painter_func;
  KPoutput_video_o output;
  const char *title;
  int width, height;
} KPwindow_params_t;

KPwindow_o kpWindowCreate(const KPwindow_params_t *params);

int appConfigure(int argc, const char *argv[]);

#ifdef __cplusplus
} // extern "C"
#endif
