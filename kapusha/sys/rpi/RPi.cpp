#include <time.h>
#include "../../core/IViewport.h"
#include "VideoCore.h"
#include "EGL.h"
#include "Evdev.h"
#include "RPi.h"

namespace kapusha {

  class RaspberryController : public IViewportController
  {
  public:
    RaspberryController(IViewport *viewport);

  public: // IViewportController
    virtual void quit(int code);
    virtual void requestRedraw() { /*! \todo */ }
    virtual void limitlessPointer(bool rel) { evdev_.setRelativeOnly(rel); }
    virtual void hideCursor(bool) { /*! \todo sw-cursor? */ }
    virtual const PointerState& pointerState() const { return evdev_.pointerState(); }
    virtual const KeyState& keyState() const { return evdev_.keyState(); }

    int run();
    int now() const;

  private:
    VideoCore videoCore_;
    EGL egl_;
    IViewport *viewport_;
    Evdev evdev_;

    bool continue_;
    int returnCode_;

  };

////////////////////////////////////////////////////////////////////////////////

  RaspberryController::RaspberryController(IViewport *viewport)
    : viewport_(viewport)
    , evdev_(viewport, videoCore_.displaySize(),
             "/dev/input/event0", "/dev/input/event1")
    , continue_(true)
  {
  }

  void RaspberryController::quit(int code)
  {
    continue_ = false;
    returnCode_ = code;
  }

  int RaspberryController::run()
  {
    EGL_DISPMANX_WINDOW_T *win = videoCore_.openWindow();
    KP_ASSERT(win);

    KP_ENSURE(egl_.init(EGL_DEFAULT_DISPLAY, win));

    viewport_->init(this);
    viewport_->resize(videoCore_.displaySize());

    int tprev = now();
    while(continue_)
    {
      evdev_.run(false);
      int tnow = now();
      viewport_->draw(tnow, (tnow - tprev) / 1000.f);
      KP_ENSURE(egl_.swap());
      tprev = tnow;
    }

    viewport_->close();
    return returnCode_;
  }

  int RaspberryController::now() const
  {
    timespec ts;
    KP_ENSURE(0 == clock_gettime(CLOCK_MONOTONIC_RAW, &ts));
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
  }

  ///////////////////////////////////////////////////////////////////////////////

  int RunRaspberryRun(IViewport *viewport)
  {
    RaspberryController rpictl(viewport);
    return rpictl.run();
  }
}
