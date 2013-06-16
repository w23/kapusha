#include "extern.h"

class GLXFBConfigSet {
public:
  enum class Buffering { None, Double };
  //! \todo enum class ColorMode { RGB565, RGB888 };
  inline GLXFBConfigSet(::Display *dpy);
  inline GLXFBConfigSet(::Display *dpy, int multisampling,
                              int color_bits = 32, int depth_bits = 24,
                              Buffering buffering = Buffering::Double);
  inline ~GLXFBConfigSet() {
    if (configurations_) XFree(configurations_);
  }

  inline int count() const { return count_; }
  inline GLXFBConfig get(int index) const {
    if (index < 0 || index >= count_) return 0;
    return configurations_[index];
  }

private:
  GLXFBConfig *configurations_;
  int count_;
};

GLXFBConfigSet::GLXFBConfigSet(::Display *dpy) {
  configurations_ = glXGetFBConfigs(dpy, 0, &count_);
}

GLXFBConfigSet::GLXFBConfigSet(::Display *dpy,
                              int multisampling,
                              int color_bits, int depth_bits,
                              Buffering buffering)
{
  int attribs[] = {
    GLX_X_RENDERABLE, True,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_CONFIG_CAVEAT, GLX_NONE,
    GLX_BUFFER_SIZE, color_bits, //!< \todo replace with individual colors
    //! \todo GLX_RED_SIZE, GLX_GREEN_SIZE, GLX_BLUE_SIZE,
    GLX_DEPTH_SIZE, depth_bits,
    GLX_DOUBLEBUFFER, (buffering == Buffering::None) ? False : True,
    GLX_SAMPLE_BUFFERS, (multisampling > 0) ? 1 : 0,
    GLX_SAMPLES, multisampling,
    0
  };
  configurations_ = glXChooseFBConfig(dpy, 0, attribs, &count_);
}
