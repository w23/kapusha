#include "GLXViewportController.h"
#include "GLXContext.h"
#include "GLXFBConfigSet.h"

namespace kapusha {
namespace sys {

GLXViewportController::~GLXViewportController() {
  if (display_) XCloseDisplay(display_);
}

int GLXViewportController::run(IViewport *viewport, vec2i size, bool fullscreen) {
  viewport_ = viewport;
  size_ = size;
  fullscreen_ = fullscreen;
  
  display_ = XOpenDisplay(NULL);
  if (display_ == NULL) {
    L("Cannot connect to X server");
    return EFAULT;
  }

  GLXFBConfigSet fbset(display_);
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
    context.makeCurrent();

    viewport_->init(this, &context);
    event_loop();
  }

  XDestroyWindow(display_, window_);
  XFree(xvis);
}

void GLXViewportController::event_loop() {
  for(;;) {
    while (XPending(display_)) {
      XEvent event;
      XNextEvent(display_, &event);
      switch (event.type) {
        case ConfigureNotify:
          viewport_->resize(vec2i(event.xconfigure.width, event.xconfigure.height));
        break;
        case Expose:
          //! \todo draw
        break;
        case KeyPress:
          return;
      }
    }

    viewport_->draw(0, 0);
    glXSwapBuffers(display_, window_);
  }
}

} // namespace sys
} // namespace kapusha
