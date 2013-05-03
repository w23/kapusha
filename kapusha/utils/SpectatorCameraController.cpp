#include "SpectatorCameraController.h"

namespace kapusha {
  SpectatorCameraController::SpectatorCameraController(Camera &camera)
    : camera_(camera), keyForward_(KeyState::KeyW), keyBack_(KeyState::KeyS)
    , keyStrafeLeft_(KeyState::KeyA), keyStrafeRight_(KeyState::KeyD)
    , keySpeed_(KeyState::KeyLeftShift), sensitivity_(1.f)
    , speed_(10.f), speedFactor_(5.f), orientation_(false)
  {
  }
  bool SpectatorCameraController::pointers(const PointerState& pointers) {
    if (orientation_ && pointers.main().wasMoved()) {
      camera_.rotatePitch(pointers.main().getDelta().y * sensitivity_);
      camera_.rotateAxis(kapusha::vec3f(0.f, 1.f, 0.f),
                         -pointers.main().getDelta().x * sensitivity_);
      return true;
    }
    return false;
  }
  bool SpectatorCameraController::frame(float dt,
                                        const IViewportController *vctrl) {
    const KeyState &k = vctrl->keyState();
    const float spd = dt*speed_*((k.isKeyPressed(keySpeed_))?speedFactor_:1.f);
    float forward = 0, right = 0;
    if (k.isKeyPressed(keyForward_)) forward += spd;
    if (k.isKeyPressed(keyBack_)) forward -= spd;
    if (k.isKeyPressed(keyStrafeRight_)) right += spd;
    if (k.isKeyPressed(keyStrafeLeft_)) right -= spd;
    if ((forward != 0) || (right != 0)) {
      camera_.moveForward(forward);
      camera_.moveRigth(right);
      return true;
    }
    return false;
  }
} // namespace kapusha