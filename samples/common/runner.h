#pragma once
#include <kapusha/window.h>

typedef struct sample_t {
  const char *title;
  void (*create)();
  void (*configure)(const KPwindow_painter_event_t *event);
  void (*paint)(const KPwindow_painter_event_t *event);
  void (*destroy)();
} sample_t;