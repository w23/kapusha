#include "runner.h"

#ifndef SAMPLES
#error Must define SAMPLES
#endif

extern sample_t SAMPLES;

#ifndef PSAMPLES
#define PSAMPLES &SAMPLES
#endif

static sample_t *samples[] = {
  PSAMPLES
};

static void painter(const KPwindow_painter_event_t *event) {
  sample_t *sample = (sample_t*)event->user_data;
  switch (event->type) {
    case KPWindowPaintBegin:
      sample->create();
      break;
    case KPWindowPaintPaint:
      sample->paint(event);
      break;
    case KPWindowPaintReconfigure:
      sample->configure(event);
      break;
    case KPWindowPaintEnd:
      if (sample->destroy)
        sample->destroy();
      break;
    case KPWindowPaintPause:
      break;

    default:
      KP_FAIL("Unexpected event %d", event->type);
      break;
  }
}

int kpuserAppCreate(int argc, const char *argv[]) {
  KP_UNUSED(argc);
  KP_UNUSED(argv);
  
  for (KPsize i = 0; i < sizeof(samples) / sizeof(*samples); ++i) {
    KPwindow_create_params_t params;
    params.event_queue = 0;
    params.paint_user_data = samples[i];
    params.painter = painter;
    params.tag = 0;
    params.title = kpStringCreate(samples[i]->title);
    KPwindow_o window = kpWindowCreate(&params);
    kpRelease(params.title);
    
    KPwindow_open_free_params_t p;
    p.min_width = 640;
    p.min_height = 480;
    p.max_width = 1920;
    p.max_height = 1080;
    
    kpWindowOpenFree(window, &p);
  }
  return 0;
}

int kpuserAppDestroy() {
  return 0;
}
