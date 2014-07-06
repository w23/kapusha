#pragma once

#include "kapusha/core.h"
#include "kapusha/math.h"

#ifdef __cplusplus
extern "C" {
#endif

struct KPsystems_i;

typedef struct {
  void *param;
  void (*dtor)(void *param);
  void (*size)(void *param, KPvec2i size);
  void (*draw)(void *param, KPtime_ms prev_present, KPtime_ms next_present);
} KPwindow_i;

typedef struct {
  void *param;
  KPwindow_i *(*init_window)(void *param, struct KPsystems_i *sys);
} KPbootstrap_t;

typedef void (*KPevent_handler_f)(void *param, const void *event);

typedef struct {
  void *param;
  KPevent_handler_f func;
} KPevent_handler_t;

enum KPRenderRate {
  KPRenderRate_OnDemand = 0,
  KPRenderRate_Native = -1,
  KPRenderRate_HalfNative = -2
};

enum KPGrabInput {
  KPGrabInput_Release = 0,
  KPGrabInput_Grab = 1
};

typedef struct KPsystems_i {
  void *param;
  int (*run)(void *param, const KPbootstrap_t *bootstrap);
  void (*set_handler)(void *param, int event_class,
      void *user_param, KPevent_handler_f func);
  void (*exit)(void *param, int code);
  /* TODO move this to KPwindow:
  void (*set_render_rate)(void *param, int rate);*/
  /* void (*grab_input)(void *param, int grab); */
} KPsystems_i;

static inline int kpSystemsRun(KPsystems_i *sys, KPbootstrap_t *bootstrap) {
  return sys->run(sys->param, bootstrap);
}

static inline void kpSystemsSetHandler(KPsystems_i *sys, int event_class,
    void *user_param, KPevent_handler_f func) {
  sys->set_handler(sys->param, event_class, user_param, func);
}

static inline void kpSystemsExit(KPsystems_i *sys, int code) {
  sys->exit(sys->param, code);
}

/* implemented by application */
int KP_main(int argc, const char *argv[], KPsystems_i *systems);

#ifdef __cplusplus
} // extern "C"
#endif
