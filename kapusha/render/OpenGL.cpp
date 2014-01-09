// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#include <cstdlib>
#include "../core/core.h"
#include "OpenGL.h"

namespace kapusha {
namespace render {
#define __V_GL_V_PAIR(a) {GL_##a,"GL_"#a}
  static struct {
    int code;
    const char* name;
  } glErrorDesc[] = {
    __V_GL_V_PAIR(INVALID_ENUM),
    __V_GL_V_PAIR(INVALID_VALUE),
    __V_GL_V_PAIR(INVALID_OPERATION),
#if !KAPUSHA_GLES
    __V_GL_V_PAIR(STACK_OVERFLOW),
    __V_GL_V_PAIR(STACK_UNDERFLOW),
#endif
    __V_GL_V_PAIR(OUT_OF_MEMORY)
#if defined GL_INVALID_FRAMEBUFFER_OPERATION
    , __V_GL_V_PAIR(INVALID_FRAMEBUFFER_OPERATION)
#endif
  };
#undef __V_GL_V_PAIR
  const char* glDescribeError(int code) {
    for(unsigned i = 0; i < sizeof(glErrorDesc)/sizeof(*glErrorDesc); ++i) {
      if (glErrorDesc[i].code == code) return glErrorDesc[i].name;
    }
    return "UNKNOWN";
  }
  void glAssert(const char* file, int line) {
    int glerr = glGetError();
    if (glerr != 0) {
      L("GL error %s [%x] @ %s:%d", glDescribeError(glerr), glerr, file, line);
      abort();
    }
  }

} // namespace render
} // namespace kapusha
