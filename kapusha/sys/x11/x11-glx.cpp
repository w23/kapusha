#include <kapusha/core.h>
#include <kapusha/viewport.h>
#include <kapusha/render/Context.h>
#include <stdio.h>
#include <errno.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>

namespace kapusha {
void log::sys_write(const char *message) {
  fprintf(stderr, "%s\n", message);
}

///////////////////////////////////////////////////////////////////////////////
class FramebufferConfigSet {
public:
  enum class Buffering {
    None, Double
  }
  inline FramebufferConfigSet(::Display *dpy);
  inline FramebufferConfigSet(::Display *dpy, int multisampling,
                              int color_bits = 32, int depth_bits = 24,
                              Buffering buffering = Buffering::Double)
  inline ~FramebufferConfigSet() {
    if (configurations_) XFree(configurations_);
  }

  inline int count() const { return count_; }
  inline GLXFBConfig *get(int index) const {
    if (index < 0 || index >= count_) return 0;
    return configurations[index];
  }

private:
  GLXFBConfig *configurations_;
  int count_;
};

FramebufferConfigSet::FramebufferConfigSet(::Display *dpy) {
  configurations_ = glXGetFBConfigs(dpy, 0, &_count);
}

FramebufferConfigSet::FramebufferConfigSet(::Display *dpy,
                              int multisampling
                              int color_bits, int depth_bits,
                              Buffering buffering)
{
  int attribs[] = {
    GLX_X_RENDERABLE, True,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_CONFIG_CAVEAT, GLX_NONE,
    GLX_BUFFER_SIZE, color_bits,
    GLX_DEPTH_SIZE, depth_bits,
    GLX_DOUBLEBUFFER, (buffering == None) ? False : True,
    GLX_SAMPLE_BUFFERS, (multisampling > 0) ? 1 : 0,
    GLX_SAMPLES, multisampling,
    0
  };
  configurations_ = glXChooseFBConfig(dpy, 0, attribs, &count_);
}

///////////////////////////////////////////////////////////////////////////////
class GLXContext : public Context {
public:
  GLXContext(::Display *display, ::GLXFBConfig fbconfig, ::GLXDrawable drawable);
  GLXContext(const GLXContext *share);
  virtual ~GLXContext();
  virtual Context *createSharedContext() const;
  virtual void makeCurrent();
private:
  void init(const GLXContext *share);
  //! \warning all these are managed outside
  //!          shared context gets same values (and the same objects)
  //!          so one should be careful and release all shared contexts before
  //!          the main one
  ::Display *display_;
  ::GLXFBConfig fbconfig_;
  ::GLXDrawable drawable_;
  ::GLXContext context_;
}; // class GLXContext

GLXContext::GLXContext(::Display *display,
                       ::GLXFBConfig fbconfig, ::GLXDrawable drawable)
  : display_(display), fbconfig_(fbconfig), drawable_(drawable) {
  init(nullptr);
}
GLXContext::GLXContext(const GLXContext *share)
  : display_(share->display_), fbconfig_(share->fbconfig_)
  , drawable_(share->drawable_) {
  init(share);
}
void GLXContext::init(const GLXContext *share) {
  //! \todo glXCreateContextAttribsARB
  context_ = glXCreateNewContext(display_, fbconfig_, GLX_RGBA_TYPE,
                                 share ? share->context_ : NULL, True);
  KP_ASSERT(context_ != NULL);
}
GLXContext::~GLXContext() {
  //! \todo unmake current
  glXDestroyContext(display_, context_);
}
Context *GLXContext::createSharedContext() {
  return new GLXContext(this);
}
void GLXContext::makeCurrent() const {
  //! \todo glXMakeContextCurrent
  Bool result = glXMakeCurrent(display_, drawable_, context_);
  KP_ASSERT(result == True);
}

///////////////////////////////////////////////////////////////////////////////
class X11ViewportController : public IViewportController {
public:
  inline X11ViewportController(IViewport *viewport, vec2i size, bool fullscreen)
    : viewport_(viewport), size_(size), fullscreen_(fullscreen), display_(0) {}
  inline ~X11ViewportController();
  inline int run();
  void event_loop();

public: // IViewportController
  void quit(int code = 0) = 0;
  void requestRedraw() = 0;
  void setRelativeOnlyPointer(bool relative_only) = 0;
  void hideCursor(bool hide) = 0;
  const PointerState& pointerState() const = 0;
  const KeyState& keyState() const = 0;

private:
  IViewport *viewport_;
  vec2i size_;
  bool fullscreen_;
  ::Display *display_;
  ::Window window_, rootWindow_;
};

inline X11ViewportController(IViewport *viewport, vec2i size, bool fullscreen)
  : viewport_(viewport), size_(size), fullscreen_(fullscreen) {
  display_ = XOpenDisplay(NULL);
}

X11ViewportController::~X11ViewportController() {
  if (display_)
    XCloseDisplay(display_);
}

int X11ViewportController::run() {
  if (display_ == NULL) {
    L("Cannot connect to X server");
    return EFAULT;
  }

  FramebufferConfigSet fbset(display_);
  GLXFBConfig fbcfg = fbset.get(0);
  if (fbcfg == NULL) {
    L("Cannot find appropriate glx framebuffer configuration");
    return EINVAL;
  }
  XVisualInfo *xvis = glXGetVisualFromFBConfig(display_, *fbcfg);
  if (xvis == NULL) {
    L("Cannot find appropriate x11 visual");
    return EINVAL;
  }

  XSetWindowAttributes watt;
  watt.event_mask = ExposureMask | VisibilityChangeMask | KeyPressMask |
                    PointerMotionMask | StructureNotifyMask;
  watt.border_pixel = 0;
  watt.bit_Gravity = StaticGravity;
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
  XFree(watt.colormap);
  XFree(xvis);
}

void X11ViewportController::event_loop() {
  for(;;) {
    while (XPending(display_)) {
      XEvent event;
      XNextEvent(display_, &event);
      switch (event->type) {
        case ConfigureNotify:
          viewport_->resize(vec2i(event->xconfigure.width, event->xconfigure.height));
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


///////////////////////////////////////////////////////////////////////////////
int X11Run(IViewport *viewport, vec2i size, bool fullscreen) {
  X11ViewportController controller(viewport, size, fullscreen);
  return controller.run();
}

} // namespace kapusha
