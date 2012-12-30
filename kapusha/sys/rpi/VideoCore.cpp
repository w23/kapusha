#include "VideoCore.h"

namespace kapusha {

  VideoCore::VideoCore()
  {
    bcm_host_init();
    uint32_t w, h;
    KP_ENSURE(graphics_get_display_size(0, &w, &h) >= 0);
    displaySize_ = vec2i(w, h);
    L("Display size: %dx%d", displaySize_.x, displaySize_.y);
    window_.element = 0;
  }

  VideoCore::~VideoCore()
  {
    //! \fixme what to do here?
    bcm_host_deinit();
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

    VC_DISPMANX_ALPHA_T alpha;
    alpha.flags = DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS;
    alpha.opacity = 255;
    alpha.mask = 0;

    dispman_display = vc_dispmanx_display_open(0);
    dispman_update = vc_dispmanx_update_start(0);
    dispman_element = vc_dispmanx_element_add(
      dispman_update, dispman_display, 0, &dst_rect, 0, &src_rect,
      DISPMANX_PROTECTION_NONE, &alpha, 0, DISPMANX_NO_ROTATE);
      
    window_.element = dispman_element;
    window_.width = displaySize_.x;
    window_.height = displaySize_.y;
    vc_dispmanx_update_submit_sync(dispman_update);

    return &window_;
  }

} // namespace kapusha
