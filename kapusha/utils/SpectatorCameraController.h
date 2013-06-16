#pragma once
#include <kapusha/ooo.h>
#include <kapusha/viewport.h>

namespace kapusha {
  class SpectatorCameraController {
  public:
    SpectatorCameraController(Camera &camera);
    void enableOrientation(bool enable) { orientation_ = enable; }
    bool pointers(const PointerState& pointers);
    bool frame(float dt, const IViewportController *vctrl);
#define KP_DECLARE_SETTER(type, name, Name) \
  inline void set##Name(type name) { name ## _ = name; }
    KP_DECLARE_SETTER(float, sensitivity, Sensitivity)
    KP_DECLARE_SETTER(float, speed, Speed)
    KP_DECLARE_SETTER(float, speedFactor, SpeedFactor)
  private:
    Camera &camera_;
    KeyState::Keys keyForward_, keyBack_;
    KeyState::Keys keyStrafeLeft_, keyStrafeRight_;
    KeyState::Keys keySpeed_;
    float sensitivity_, speed_, speedFactor_;
    bool orientation_;
  };
} // namespace kapusha