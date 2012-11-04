#pragma once

#ifdef WIN32
#include <GL/glew.h>
#include <GL/gl.h>
#elif defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#elif defined(__linux__)
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
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
