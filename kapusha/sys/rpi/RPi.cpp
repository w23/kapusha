#include <cstdio>
#include <time.h>
#include <kapusha/render/Context.h>
#include "RPi.h"

namespace kapusha {
  void log::sys_write(const char *message) {
    fprintf(stderr, "%s\n", message);
  }

  RaspberryController::RaspberryController()
    : evdev_(video_core_.displaySize(),
             "/dev/input/event0", "/dev/input/event1")
    , continue_(true), return_code_(0) {}

  void RaspberryController::quit(int code) {
    continue_ = false;
    return_code_ = code;
  }

  int RaspberryController::run(IViewportFactory *factory) {
    EGL_DISPMANX_WINDOW_T *win = video_core_.openWindow();
    KP_ASSERT(win);

    KP_ENSURE(egl_.init(EGL_DEFAULT_DISPLAY, win));

    {
      RaspberryContext context;
      IViewport *viewport = factory->create(this);
      viewport->resize(video_core_.displaySize());
      evdev_.set_viewport(viewport);

      int tprev = now();
      while(continue_)
      {
        evdev_.run(false);
        int tnow = now();
        viewport->draw(tnow, (tnow - tprev) / 1000.f);
        KP_ENSURE(egl_.swap());
        tprev = tnow;
      }

      delete viewport;
    }
    return return_code_;
  }

  int RaspberryController::now() const {
    timespec ts;
    KP_ENSURE(0 == clock_gettime(CLOCK_MONOTONIC_RAW, &ts));
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
  }
}
