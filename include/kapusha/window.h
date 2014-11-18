#pragma once
#include <kapusha/app.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *KPwindow_o;

typedef struct {
  enum {
    KPWindowPaintBegin,
    KPWindowPaintReconfigure,
    KPWindowPaintPaint,
    KPWindowPaintPause,
    KPWindowPaintEnd
  } type;
  KPwindow_o window;
  void *user_data;
  struct {
    KPu32 width, height;
    KPf32 aspect;
  } configuration;
  struct {
    KPtime_ns pts;
    KPtime_ns time_delta;
    KPtime_ns time_delta_frame;
  } time;
} KPwindow_painter_event_t;

typedef void (*KPwindow_painter_f)(const KPwindow_painter_event_t *);

typedef struct KPwindow_free_params_t {
  KPu32 min_width, min_height;
  KPu32 max_width, max_height;
} KPwindow_free_params_t;

KPwindow_o kpWindowCreate(void *user_data, KPwindow_painter_f, KPstring_o title);
int kpWindowOpenFree(KPwindow_o, const KPwindow_free_params_t *params);
int kpWindowOpenAttached(KPwindow_o, KPoutput_video_o);

#ifdef __cplusplus
} // extern "C"
#endif
