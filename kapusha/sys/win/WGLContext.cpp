#include "WGLContext.h"

#define KP_GL_PROC(type, name) type name = NULL
  KP_GL_PROCS
#undef KP_GL_PROC

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

void WGLContext::load_procs() {
#define KP_GL_PROC(type,name) name = reinterpret_cast<type>(wglGetProcAddress(#name))
  KP_GL_PROCS
#undef GL_PROC
}
} // namespace kapusha