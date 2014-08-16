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
#include "3p/gl/glext.h"
#include <kapusha/sys/win/gl_proc.h>
#elif defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE || TARGET_OS_IPHONE_SIMULATOR
#define KP_GLES 1
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#endif
#elif KP_RASPBERRY
#define KP_GLES 1
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif defined(__linux__)
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#else
#error please include opengl in platform-specific way
#endif

#ifdef DEBUG
void kp__GlAssert(const char *file, int line);
#define KP__GLASSERT kp__GlAssert(__FILE__, __LINE__);
#else
#define KP__GLASSERT
#endif

#include "kapusha/render.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* GL buffer */

typedef enum {
  KP__RenderBufferTargetArray,
  KP__RenderBufferTargetElementArray,
  KP__RenderBufferTarget_MAX
} KP__RenderBufferTarget;

typedef struct { KP_O;
  GLuint name;
} KP__render_buffer_t;

/******************************************************************************/
/* GL program */

enum {
  KP__RenderProgramHaveVertex = 0x01,
  KP__RenderProgramHaveFragment = 0x02,
  KP__RenderProgramReady = 0x03,
} KP__RenderProgramFlags;

enum {
  KP__RenderProgramShaderVertex,
  KP__RenderProgramShaderFragment,
  KP__RenderProgramShader_MAX
} KP__RenderProgramShader;

#define KP__RENDER_PROGRAM_MAX_ARGS 8
#define KP__RENDER_BATCH_MAX_ATTRIBS 8

typedef struct { KP_O;
  GLuint name;
  KPu32 flags;
  GLuint shaders[KP__RenderProgramShader_MAX];

  struct {
    KPrender_tag_t tag;
    int location;
  } args[KP__RENDER_PROGRAM_MAX_ARGS];

  struct {
    KPrender_tag_t tag;
    int location;
  } attribs[KP__RENDER_BATCH_MAX_ATTRIBS];
} KP__render_program_t;

/******************************************************************************/
/* GL program environment */

typedef enum {
  KP__RenderProgramEnvValueNone,
  KP__RenderProgramEnvValueScalarf,
  KP__RenderProgramEnvValueVec4f,
  KP__RenderProgramEnvValueMat4f,
} KP__RenderProgramEnvValueType;

#define KP__RENDER_PROGRAM_ENV_MAX_VALUES 8

typedef struct { KP_O;
  struct KP__render_program_env_value_t {
    KPrender_tag_t tag;
    KP__RenderProgramEnvValueType type;
    union {
      KPscalarf f[16];
    } v;
  } values[KP__RENDER_PROGRAM_ENV_MAX_VALUES];
} KP__render_program_env_t;

/******************************************************************************/
/* GL batch */

typedef struct { KP_O;
  struct {
    KPrender_tag_t tag;
    KPrender_vertex_attrib_t attrib;
  } attribs[KP__RENDER_BATCH_MAX_ATTRIBS];
  struct {
    KP__render_buffer_t *buffer;
    KPu32 primitive, type;
    KPu32 count;
    KPuptr offset;
  } index;
} KP__render_batch_t;

/******************************************************************************/
/* GL render state */

typedef struct {
  /* are not retained */
  KPheap_p heap;
  KP__render_buffer_t *buffer_targets_[KP__RenderBufferTarget_MAX];
  KP__render_program_t *program_;
} KP__render_state_t;

#ifdef __cplusplus
} // extern "C"
#endif
