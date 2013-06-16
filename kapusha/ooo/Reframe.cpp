#include "Reframe.h"

namespace kapusha {
  Reframe::Reframe(vec3f pos, vec3f forward, vec3f up) : translation_(pos) {
    setOrientation(forward, up);
    update();
  }
  void Reframe::setOrientation(vec3f forward, vec3f up) {
    forward_ = forward.normalize();
    right_ = forward_.cross(up.normalize());
    up_  = right_.cross(forward_);
  }
  void Reframe::rotatePitch(float radians) {
    mat4f rotation(mat4f().makeRotation(right_, radians));
    forward_ = (rotation * forward_).normalize();
    up_ = right_.cross(forward_);
  }
  void Reframe::rotateRoll(float radians) {
    mat4f rotation(mat4f().makeRotation(forward_, radians));
    up_ = (rotation * up_).normalize();
    right_ = forward_.cross(up_);
  }
  void Reframe::rotateYaw(float radians) {
    mat4f rotation(mat4f().makeRotation(up_, radians));
    forward_ = (rotation * forward_).normalize();
    right_ = forward_.cross(up_);
  }
  void Reframe::rotateAxis(vec3f axis, float radians) {
    mat4f rotation(mat4f().makeRotation(axis, radians));
    forward_ = (rotation * forward_).normalize();
    right_ = (rotation * right_).normalize();
    up_ = right_.cross(forward_);
    right_ = forward_.cross(up_);
  }
  void Reframe::update() {
    combined_ = mat4f(vec4f(right_), vec4f(up_), vec4f(-forward_))
      * mat4f().makeTranslation(translation_);
  }
} // namespace kapusha
