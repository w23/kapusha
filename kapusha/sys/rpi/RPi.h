#pragma once
#include <kapusha/viewport.h>
#include "VideoCore.h"
#include "EGL.h"
#include "Evdev.h"

namespace kapusha {
  class RaspberryController : public IViewportController {
  public:
    RaspberryController();

  public: // IViewportController
    virtual void quit(int code);
    virtual void grab_input(bool grab) {}
    virtual const Pointers& pointers() const { return evdev_.pointers(); }
    virtual const Keys& keys() const { return evdev_.keys(); }

    int run(IViewportFactory *factory);
    int now() const;

  private:
    VideoCore video_core_;
    EGL egl_;
    Evdev evdev_;

    bool continue_;
    int return_code_;
  };

  /// \todo fix dummy
  class RaspberryContext : public Context {
  public:
    RaspberryContext() {}
  };
} // namespace kapusha
