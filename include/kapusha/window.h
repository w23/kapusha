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

typedef struct KPwindow_event_t {
  enum {
    KPWindowEventVisibility,
    KPWindowEventFocus,
    KPWindowEventCloseRequest,
    KPWindowEventDestroyed,
    KPWindowEventKeyboard,
    KPWindowEventPointer
  } type;
} KPwindow_event_t;

typedef struct KPwindow_create_params_t {
  KPstring_o title;

  KPdisplay_o attachment;

  KPwindow_painter_f painter;
  void *paint_user_data;
  
  KPmessage_queue_t queue;
  KPmessage_user_t queue_userdata;
} KPwindow_create_params_t;

KPwindow_o kpWindowCreate(const KPwindow_create_params_t *params);
void kpWindowStart(KPwindow_o);
void kpWindowRepaint(KPwindow_o);
void kpWindowStop(KPwindow_o);

#ifdef __cplusplus
} // extern "C"
#endif
