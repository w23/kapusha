#include "../../render/OpenGL.h"
#include "RPi.h"
#include <bcm_host.h>

namespace kapusha {

  RPiGLES::RPiGLES()
  {
    bcm_host_init();
  }

  bool RPiGLES::initGL(u32 width, u32 height)
  {
    if (0 != graphics_get_display_size(0, &width_, &height_))
      return false;
    L("Native size: %dx%d", width_, height_);
    width_ = width; height_ = height;

    DISPMANX_DISPLAY_HANDLE_T h_display = vc_dispmanx_display_open(0);
    DISPMANX_UPDATE_HANDLE_T h_update = vc_dispmanx_update_start(0);
    
    VC_RECT_T src_rect, dst_rect;
    src_rect.x = src_rect.y = dst_rect.x = dst_rect.y = 0;
    dst_rect.width = width_;
    dst_rect.height = height_;
    src_rect.width = width_ << 16;
    src_rect.height = height_ << 16;

    EGL_DISPMANX_WINDOW_T window;
    window.width = width_;
    window.height = height_;
    window.element = vc_dispmanx_element_add(h_update, h_display,
      0, &dst_rect, 0, &src_rect, DISPMANX_PROTECTION_NONE, 0, 0,
      static_cast<DISPMANX_TRANSFORM_T>(0));
    vc_dispmanx_update_submit_sync(h_update);

    return initEGL(&window);
  }

  bool RPiGLES::initEGL(void* window)
  {
#define FAIL(c) {\
L("%s:%d: %d", __FILE__, __LINE__, c);\
return false;\
}
    display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display_ == EGL_NO_DISPLAY) FAIL(display_)

    EGLint v_major, v_minor;
    if (!eglInitialize(display_, &v_major, &v_minor)) FAIL(0)
    L("GLES v%d.%d", v_major, v_minor);

    EGLint configs;
    if (!eglGetConfigs(display_, NULL, 0, &configs)) FAIL(0)
    L("Configs: %d", configs);

    EGLConfig config;
    EGLint surface_attrs[] = {
      EGL_RED_SIZE, 5, EGL_GREEN_SIZE, 6, EGL_BLUE_SIZE, 5,
      EGL_ALPHA_SIZE, EGL_DONT_CARE,
      EGL_DEPTH_SIZE, 16, EGL_STENCIL_SIZE, EGL_DONT_CARE,
      EGL_SAMPLE_BUFFERS, 0,
      EGL_NONE
    };
    if (!eglChooseConfig(display_, surface_attrs, &config, 1, &configs)) FAIL(0)

    surface_ = eglCreateWindowSurface(display_, config, (EGLNativeWindowType)window, NULL);
    if (surface_ == EGL_NO_SURFACE) FAIL(0)

    EGLint attrs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    EGLContext context = eglCreateContext(display_, config, EGL_NO_CONTEXT, attrs);
    if (context == EGL_NO_CONTEXT) FAIL(0)
   
    if (!eglMakeCurrent(display_, surface_, surface_, context)) FAIL(0)
    GL_ASSERT
    return true;
  }

  void RPiGLES::swap() const
  {
    GL_ASSERT
    eglSwapBuffers(display_, surface_);
    GL_ASSERT
  }

  void RPiGLES::closeGL()
  {
    //! #fixme
  }

  RPiGLES::~RPiGLES()
  {
    bcm_host_deinit();
  }

}
