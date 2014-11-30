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

int kpuserAppRun(int argc, const char *argv[]) {
  KP_UNUSED(argc);
  KP_UNUSED(argv);
  
  KPmessage_queue_t app_queue = kpMessageQueueCreate();
  
  int opened_windows;
  for (KPsize i = 0; i < sizeof(samples) / sizeof(*samples); ++i) {
    KPwindow_create_params_t params;
    params.title = kpStringCreate(samples[i]->title);
    params.attachment = 0;
    params.painter = painter;
    params.paint_user_data = samples[i];
    params.queue = app_queue;
    params.queue_userdata.data = 0;
    params.queue_userdata.tag = 0;
    KPwindow_o window = kpWindowCreate(&params);
    kpRelease(params.title);
    kpWindowStart(window);
    ++opened_windows;
  }
  
  int loop = 1;
  while (loop) {
    KPmessage_t *msg = kpMessageQueueGet(app_queue, -1);
    KP_ASSERT(msg);
    KP_L("msg: %d, from %p", msg->type, msg->origin);
    if (msg->user.tag == 0) {
      switch (msg->type) {
      case KPWindowEventCloseRequest:
        kpRelease(msg->origin);
        break;
      case KPWindowEventDestroyed:
        --opened_windows;
        if (opened_windows == 0)
          loop = 0;
        break;
      }
    }
    kpMessageRelease(msg);
  }
  
  kpMessageQueueDestroy(app_queue);
  return 0;
}
