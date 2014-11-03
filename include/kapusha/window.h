#pragma once
#include "kapusha/core.h"

typedef struct KPoutput_t { KP_O;
  enum {
    KPOutputVideo
  } type;
} KPoutput_t, *KPoutput_o;

typedef struct KPoutput_video_t { KPoutput_t header;
  int width, height;
  int width_mm, height_mm;
  KPu32 frame_delta_usec;
  KPu32 flags;
} KPoutput_video_t, *KPoutput_video_o;

typedef void *KPwindow_o;

typedef struct {
  enum {
    KPWindowEventCreate,
    KPWindowEventGeometry,
    KPWindowEventPaint
  } type;

  union {
    struct {
      void *opaque;
      KPwindow_o window;
    } create;

    struct {
      int x, y;
      int width, height;
    } geometry;

    struct {
      KPtime_ns pts_estimate;
      KPtime_ns time_delta;
      KPtime_ns time_delta_frame;
    } paint;
  } info;
} KPwindow_event_t;

typedef void (*KPwindow_paint_f)(const KPwindow_event_t *event);

typedef struct {
  void *user_data;
  KPwindow_paint_f paint_func;
  KPoutput_video_o output;
  const char *title;
  int width, height;
} KPwindow_params_t;

KPwindow_o kpWindowCreate(const KPwindow_params_t *params);
