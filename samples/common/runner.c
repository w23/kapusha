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

static KPmessage_queue_t app_queue;

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

static KPthread_t queue_thread;
static void *queue_thread_func(void *param) {
  int loop = 1;
  while (loop) {
    KPmessage_t *msg = kpMessageQueueGet(app_queue, -1);
    if (msg->tag == -1) {
      loop = 0;
    }
    kpMessageDiscard(msg);
  }
  return 0;
}

int kpuserAppCreate(int argc, const char *argv[]) {
  KP_UNUSED(argc);
  KP_UNUSED(argv);
  
  app_queue = kpMessageQueueCreate();
  
  for (KPsize i = 0; i < sizeof(samples) / sizeof(*samples); ++i) {
    KPwindow_create_params_t params;
    params.event_queue = app_queue;
    params.paint_user_data = samples[i];
    params.painter = painter;
    params.tag = (KPu32)i;
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
  
  KPthread_params_t tp;
  tp.user_data = 0;
  tp.thread_func = queue_thread_func;
  tp.name = "runner queue";
  queue_thread = kpThreadSpawn(&tp);
  return 0;
}

int kpuserAppDestroy() {
  kpMessageQueuePut(app_queue, -1, 0, 0, 0, 0, 0);
  kpThreadJoin(queue_thread);
  kpMessageQueueDestroy(app_queue);
  return 0;
}
