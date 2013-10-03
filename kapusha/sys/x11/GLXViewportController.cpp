#include "GLXViewportController.h"
#include "GLXContext.h"
#include "GLXFBConfigSet.h"

namespace kapusha {
namespace sys {

class ClockMonotonic {
public:
  static u64 now() {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL;
  }
};

GLXViewportController::~GLXViewportController() {
  if (display_) XCloseDisplay(display_);
}

int GLXViewportController::run(const IViewportFactory *factory) {
  const IViewportFactory::Preferences &prefs = factory->preferences();

  vec2i size = prefs.prefer_resolution;
  if (size.x <= 0 || size.y <= 0) size = vec2i(640, 480);

  display_ = XOpenDisplay(NULL);
  if (display_ == NULL) {
    L("Cannot connect to X server");
    return EFAULT;
  }

  GLXFBConfigSet fbset(display_);
  //! \todo Implement config selection based on factory prefs
  GLXFBConfig fbcfg = fbset.get(0);
  if (fbcfg == NULL) {
    L("Cannot find appropriate glx framebuffer configuration");
    return EINVAL;
  }
  XVisualInfo *xvis = glXGetVisualFromFBConfig(display_, fbcfg);
  if (xvis == NULL) {
    L("Cannot find appropriate x11 visual");
    return EINVAL;
  }

  XSetWindowAttributes watt;
  watt.event_mask = ExposureMask | VisibilityChangeMask | KeyPressMask |
                    PointerMotionMask | StructureNotifyMask;
  watt.border_pixel = 0;
  watt.bit_gravity = StaticGravity;
  watt.colormap = XCreateColormap(display_, RootWindow(display_, xvis->screen),
                                  xvis->visual, AllocNone);

  window_ = XCreateWindow(display_, DefaultRootWindow(display_),
                          0, 0, size.x, size.y, 0,
                          xvis->depth, InputOutput, xvis->visual,
                          CWBorderPixel | CWBitGravity | CWEventMask | CWColormap,
                          &watt);
  XMapWindow(display_, window_);

  //! \todo glXCreateWindow
  
  {
    GLXContext context(display_, fbcfg, window_);
    context.make_current();

    viewport_ = factory->create(this);
    event_loop();
    delete viewport_;
    viewport_ = nullptr;
  }

  XDestroyWindow(display_, window_);
  XFree(xvis);
  return 0;
}

void GLXViewportController::event_loop() {
  u64 start = ClockMonotonic::now(), prev = 0;
  for(;;) {
    while (XPending(display_)) {
      XEvent event;
      XNextEvent(display_, &event);
      switch (event.type) {
        case ConfigureNotify:
          {
            vec2i size = vec2i(event.xconfigure.width, event.xconfigure.height);
            if (size_ != size) {
              size_ = size;
              viewport_->resize(size_);
            }
          }
        break;
        case Expose:
          //! \todo draw
        break;
        //case KeyPress:
        //  return;
      }
    }

    u64 now = ClockMonotonic::now() - start;
    viewport_->draw(now, (now-prev) / 1000.f);
    glXSwapBuffers(display_, window_);
    prev = now;
  }
}

} // namespace sys
} // namespace kapusha
