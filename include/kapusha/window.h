#pragma once
#include <kapusha/app.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *KPwindow_o;

typedef struct {
  KPwindow_o window;
  void *user_data;
} KPwindow_painter_header_t;

typedef struct {
  KPwindow_painter_header_t header;
  KPu32 width, height;
  KPf32 aspect;
} KPwindow_painter_configure_t;

typedef struct {
  KPwindow_painter_configure_t config;
  KPtime_ns pts;
  KPtime_ns time_delta;
  KPtime_ns time_delta_frame;
} KPwindow_painter_paint_t;

typedef void (*KPwindow_painter_create_f)(const KPwindow_painter_header_t *);
typedef void (*KPwindow_painter_configure_f)(const KPwindow_painter_configure_t *);
typedef void (*KPwindow_painter_f)(const KPwindow_painter_paint_t *);
typedef void (*KPwindow_painter_destroy_f)(const KPwindow_painter_header_t *);

enum {
  KPWindowFlagFullscreen = 0x01,
  KPWindowFlagFixedSize = 0x02,
  KPWindowFlagFixedPos = 0x04
} KPWindowFlags;

typedef struct {
  void *user_data;
  KPwindow_painter_create_f painter_create_func;
  KPwindow_painter_configure_f painter_configure_func;
  KPwindow_painter_f painter_func;
  KPwindow_painter_destroy_f painter_destroy_func;
  KPoutput_video_o output;
  const char *title;
  int width, height;
  KPu32 flags;
} KPwindow_params_t;

KPwindow_o kpWindowCreate(const KPwindow_params_t *params);

#ifdef __cplusplus
} // extern "C"
#endif
