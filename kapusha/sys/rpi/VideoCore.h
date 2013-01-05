#pragma once

#include <bcm_host.h>
#include "EGL.h"
#include "../../core/Core.h"
#include "../../math/types.h"

namespace kapusha {

  class VideoCore {
  public:
    VideoCore();
    ~VideoCore();

    vec2i displaySize() const { return displaySize_; }

    //! \todo floating window with some pos+size
    EGL_DISPMANX_WINDOW_T *openWindow();

  private:
    vec2i displaySize_;
    EGL_DISPMANX_WINDOW_T window_;
  };

} // namespace kapusha
