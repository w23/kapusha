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
  private:
    Camera &camera_;
    KeyState::Keys keyForward_, keyBack_;
    KeyState::Keys keyStrafeLeft_, keyStrafeRight_;
    KeyState::Keys keySpeed_;
    float sensitivity_, speed_, speedFactor_;
    bool orientation_;
  };
} // namespace kapusha