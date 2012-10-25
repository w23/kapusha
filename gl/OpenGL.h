#pragma once

#ifdef WIN32
#include <GL/glew.h>
#include <GL/gl.h>
#else
#error please include opengl in platform-specific way
#endif

#if DEBUG
namespace kapusha {
  void glAssert(const char*, int);
}
#define GL_ASSERT glAssert(__FILE__,__LINE__);
#else
#define GL_ASSERT {}
#endif