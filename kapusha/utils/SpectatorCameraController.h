#pragma once
#include <kapusha/ooo.h>
#include <kapusha/viewport.h>

namespace kapusha {
  class SpectatorCameraController {
  public:
    SpectatorCameraController(ooo::camera_t &camera);
    void enable_orientation(bool enable) { orientation_ = enable; }
    bool pointers(const Pointers &pointers);
    bool frame(float dt, const IViewportController *vctrl);
#define KP_DECLARE_SETTER(type, name) \
  inline void set_##name(type name) { name ## _ = name; }
    KP_DECLARE_SETTER(float, sensitivity)
    KP_DECLARE_SETTER(float, speed)
    KP_DECLARE_SETTER(float, speed_factor)
#undef KP_DECLARE_SETTER
  private:
    ooo::camera_t &camera_;
    Keys::Types key_forward_, key_back_;
    Keys::Types key_strafe_left_, key_strafe_right_;
    Keys::Types key_speed_;
    float sensitivity_, speed_, speed_factor_;
    bool orientation_;
  };
} // namespace kapusha
