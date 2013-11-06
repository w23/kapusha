#include "SpectatorCameraController.h"

namespace kapusha {
  SpectatorCameraController::SpectatorCameraController(Camera &camera)
    : camera_(camera), key_forward_(Keys::KeyW), key_back_(Keys::KeyS)
    , key_strafe_left_(Keys::KeyA), key_strafe_right_(Keys::KeyD)
    , key_speed_(Keys::KeyLeftShift), sensitivity_(1.f)
    , speed_(10.f), speed_factor_(5.f), orientation_(false)
  {
  }
  bool SpectatorCameraController::pointers(const Pointers &pointers) {
    if (orientation_ && pointers.main().was_moved()) {
      camera_.rotatePitch(pointers.main().delta().y * sensitivity_);
      camera_.rotateAxis(kapusha::vec3f(0.f, 1.f, 0.f),
                         -pointers.main().delta().x * sensitivity_);
      return true;
    }
    return false;
  }
  bool SpectatorCameraController::frame(float dt,
                                        const IViewportController *vctrl) {
    const Keys &k = vctrl->keys();
    const float spd = dt*speed_*((k.is_key_pressed(key_speed_))?speed_factor_:1.f);
    float forward = 0, right = 0;
    if (k.is_key_pressed(key_forward_)) forward += spd;
    if (k.is_key_pressed(key_back_)) forward -= spd;
    if (k.is_key_pressed(key_strafe_right_)) right += spd;
    if (k.is_key_pressed(key_strafe_left_)) right -= spd;
    if ((forward != 0) || (right != 0)) {
      camera_.moveForward(forward);
      camera_.moveRigth(right);
      return true;
    }
    return false;
  }
} // namespace kapusha
