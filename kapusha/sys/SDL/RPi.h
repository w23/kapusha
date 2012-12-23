#include <EGL/egl.h>
#include "../../core/Core.h"

namespace kapusha {

  class RPiGLES {
  public:
    RPiGLES();
    ~RPiGLES();

    bool initGL(u32 width, u32 height);
    void closeGL();

    u32 width() const { return width_; }
    u32 height() const { return height_; }

    void swap() const;

  private:
    bool initEGL(void* window);

    u32 width_, height_;
    EGLDisplay display_;
    EGLSurface surface_; 
  };

} // namespace kapusha
