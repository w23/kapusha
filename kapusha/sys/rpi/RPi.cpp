#include "RPi.h"
#include <bcm_host.h>

namespace kapusha {

  VideoCore::VideoCore()
  {
    bcm_host_init();
    KP_ENSURE(graphics_get_display_size(0,
                                        &displaySize_.x,
                                        &displaySize_.y) >= 0);
    window_.element = 0;
  }

  VideoCore::~VideoCore()
  {
    //! \fixme what to do here?
  }

  EGL_DISPMANX_WINDOW_T *VideoCore::openWindow()
  {
    KP_ASSERT(!window_.element);
    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;
    
    dst_rect.x = dst_rect.y = 0;
    dst_rect.width = displaySize_.x;
    dst_rect.height = displaySize_.y;
      
    src_rect.x = src_rect.y = 0;
    src_rect.width = displaySize_.x << 16;
    src_rect.height = displaySize_.y << 16;

    dispman_display = vc_dispmanx_display_open(0);
    dispman_update = vc_dispmanx_update_start(0);
    dispman_element = vc_dispmanx_element_add(
      dispman_update, dispman_display, 0, &dst_rect, 0, &src_rect,
      DISPMANX_PROTECTION_NONE, 0, 0, DISPMANX_NO_ROTATE);
      
    window_.element = dispman_element;
    window_.width = displaySize_.x;
    window_.height = displaySize_.y;
    vc_dispmanx_update_submit_sync(dispman_update);

    return &window_;
  }

///////////////////////////////////////////////////////////////////////////////

  bool RPi::initGL(vec2u& size)
  {
    size = videocore_.displaySize();
    EGL_DISPMANX_WINDOW_T *win = videocore_.openWindow();
    KP_ASSERT(win);

    return egl_.init(EGL_DEFAULT_DISPLAY, win);
  }

  void RPi::closeGL()
  {
    //! \fixme what here?
  }

  bool RPi::swap() const
  {
    return egl_.swap();
  }
}
