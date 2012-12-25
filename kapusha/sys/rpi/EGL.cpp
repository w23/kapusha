#include "../../core/Core.h"
#include "EGL.h"

namespace kapusha {

#define INT_STRING(a) {EGL_##a,"EGL_"#a}
  const EGL::IntString EGL::s_about[] = {
    INT_STRING(VERSION),
    INT_STRING(VENDOR),
    INT_STRING(CLIENT_APIS),
    INT_STRING(EXTENSIONS)
  };

  const EGL::IntString EGL::s_errors[] = {
    INT_STRING(NOT_INITIALIZED),
    INT_STRING(BAD_ACCESS),
    INT_STRING(BAD_ALLOC),
    INT_STRING(BAD_ATTRIBUTE),
    INT_STRING(BAD_CONFIG),
    INT_STRING(BAD_CONTEXT),
    INT_STRING(BAD_CURRENT_SURFACE),
    INT_STRING(BAD_DISPLAY),
    INT_STRING(BAD_MATCH),
    INT_STRING(BAD_NATIVE_PIXMAP),
    INT_STRING(BAD_NATIVE_WINDOW),
    INT_STRING(BAD_PARAMETER),
    INT_STRING(BAD_SURFACE),
    INT_STRING(CONTEXT_LOST)
  };
#undef INT_STRING

  const EGLint EGL::s_config_attrs[] =
  {
      EGL_RED_SIZE, 5,
      EGL_GREEN_SIZE, 6,
      EGL_BLUE_SIZE, 5,
      EGL_DEPTH_SIZE, 16,
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_NONE
   };
   
   const EGLint EGL::s_context_attrs[] = 
   {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE
   };

  EGL::EGL()
    : display_(EGL_NO_DISPLAY)
    , context_(EGL_NO_CONTEXT)
    , surface_(EGL_NO_SURFACE)
  {
  }

  EGL::~EGL()
  {
    close();
  }

  bool EGL::init(EGLNativeDisplayType display,
                 EGLNativeWindowType window)
  {
#define FAIL(msg) do{fail(msg);KP_ASSERT(!msg);return false;}while(0);
    display_ = eglGetDisplay(display);
    if (EGL_NO_DISPLAY == display_) FAIL("eglGetDisplay");

    EGLBoolean result;
    result = eglInitialize(display_, &versionMajor_, &versionMinor_);
    if (EGL_FALSE == result) FAIL("eglInitialize");

    about();

    EGLConfig config;
    EGLint num_config;
    result = eglChooseConfig(display_, s_config_attrs,
                             &config, 1, &num_config);
    if (EGL_FALSE == result) FAIL("eglChooseConfig");

    context_ = eglCreateContext(display_, config,
                                EGL_NO_CONTEXT, s_context_attrs);
    if (EGL_NO_CONTEXT == context_) FAIL("eglCreateContext");

    surface_ = eglCreateWindowSurface(display_, config, window, 0);
    if (EGL_NO_SURFACE == surface_) FAIL("eglCreateWindowSurface");

    result = eglMakeCurrent(display_, surface_, surface_, context_);
    if (EGL_FALSE == result) FAIL("eglMakeCurrent");

    return true;
  }

  bool EGL::swap() const
  {
    return EGL_TRUE == eglSwapBuffers(display_, surface_);
  }

  void EGL::close()
  {
    //! \fixme delete little shits
    if (display_ != EGL_NO_DISPLAY)
    {
      eglTerminate(display_);
      display_ = EGL_NO_DISPLAY;
    }
  }

  void EGL::about() const
  {
    L("EGL version %d.%d", versionMajor_, versionMinor_);
    const int nstrings = sizeof(s_about) / sizeof(*s_about);
    for (int i = 0; i < nstrings; ++i)
      L("%s: %s", s_about[i].s, eglQueryString(display_,s_about[i].n));

    //! \fixme configs
  }

  void EGL::fail(const char *msg) const
  {
    L("EGL::fail: %s", msg);
  }

} // namespace kapusha
