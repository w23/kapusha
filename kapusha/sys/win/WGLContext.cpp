#include "WGLContext.h"

namespace kapusha {
  WGLContext::WGLContext(HDC dc) : dc_(dc) {
    static const PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR), 1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
      32, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 32, 0, 0,
      PFD_MAIN_PLANE, 0, 0, 0, 0 };
    SetPixelFormat(dc, ChoosePixelFormat(dc_, &pfd), &pfd);
    glrc_ = wglCreateContext(dc_);
  }

  WGLContext::~WGLContext() {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(glrc_);
  }

  void WGLContext::make_current() {
    wglMakeCurrent(dc_, glrc_);
    Context::make_current();
  }
} // namespace kapusha