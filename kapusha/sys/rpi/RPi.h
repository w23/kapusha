#include "../../core/Core.h"
#include "../../math/types.h"
#include "EGL.h"

namespace kapusha {

  typedef vec2<u32> vec2u;

  class VideoCore {
  public:
    VideoCore();
    ~VideoCore();

    vec2u displaySize() const { return displaySize_; }

    //! \todo floating window with some pos+size
    EGL_DISPMANX_WINDOW_T *openWindow();

  private:
    vec2u displaySize_;
    EGL_DISPMANX_WINDOW_T window_;
  };

  class RPi {
  public:
    RPi() {}
    ~RPi() {}

    bool initGL(vec2u& size);
    void closeGL();

    bool swap() const;

  private:
    VideoCore videocore_;
    EGL egl_;
  };

} // namespace kapusha
