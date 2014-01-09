// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <GL/gl.h>
#include <3p/gl/glext.h>
#include <kapusha/sys/win/gl_proc.h>
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
#elif KP_RASPBERRY
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
namespace render {
#if DEBUG
  void glAssert(const char*, int);
#define GL_ASSERT ::kapusha::render::glAssert(__FILE__,__LINE__);
#else
#define GL_ASSERT {}
#endif
} // namespace render
} // namespace kapusha
