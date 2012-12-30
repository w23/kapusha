#pragma once

#ifdef WIN32
#include <GL/glew.h>
#include <GL/gl.h>
#elif defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE || TARGET_OS_IPHONE_SIMULATOR
#define KAPUSHA_GLES 1
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#endif
#elif KAPUSHA_RPI
#define KAPUSHA_GLES 1
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif defined(__linux__)
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#else
#error please include opengl in platform-specific way
#endif

namespace kapusha {

#if DEBUG
  void glAssert(const char*, int);
#define GL_ASSERT ::kapusha::glAssert(__FILE__,__LINE__);
#else
#define GL_ASSERT {}
#endif
  
} // namespace kapusha
