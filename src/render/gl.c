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

#include "kapusha/render.h"

void kpRenderSetDestination(const KPrender_destination_t *dest) {
  glViewport(
    dest->viewport.bl.x, dest->viewport.bl.y,
    dest->viewport.tr.x - dest->viewport.bl.x,
    dest->viewport.tr.y - dest->viewport.bl.y);
}

void kpRenderExecuteCommand(const KPrender_cmd_header_t *command) {
  switch (command->cmd) {
  case KPrender_Command_Fill:
    {
      KPrender_cmd_fill_t *cmd = (KPrender_cmd_fill_t*)command;
      glClearColor(cmd->color.x, cmd->color.y, cmd->color.z, cmd->color.w);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    break;
  case KPrender_Command_Rasterize:
    KP_ASSERT(!"Not implemented");
    break;
    
  default:
    KP_ASSERT(!"Invalid command");
  }
}
