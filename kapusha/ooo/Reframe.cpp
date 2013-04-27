#include "Reframe.h"

namespace kapusha {
  
  Reframe::Reframe(vec3f pos, vec3f forward, vec3f up)
    : translation_(pos) {
    orient(forward, up);
    update();
  }
  
  void Reframe::orient(vec3f forward, vec3f up) {
    forward_ = vec4f(forward.normalize());
    right_ = forward_.cross(vec4f(up.normalize()));
    up_  = right_.cross(forward_);
  }
  void Reframe::lookAt(vec3f pos, vec3f at, vec3f up) {
    translate(pos);
    orient(at - pos, up);
  }
  void Reframe::moveForward(float units) {
    translation_ += forward_ * units;
  }
  void Reframe::moveRigth(float units) {
    translation_ += right_ * units;
  }
  void Reframe::moveUp(float units) {
    translation_ += up_ * units;
  }

  void Reframe::rotatePitch(float radians) {
    mat4f rotation;
    rotation.makeRotation(right_, radians);
    forward_ = (rotation * forward_).normalize();
    up_ = right_.cross(forward_);
  }
  void Reframe::rotateRoll(float radians) {
    mat4f rotation;
    rotation.makeRotation(forward_, radians);
    up_ = (rotation * up_).normalize();
    right_ = forward_.cross(up_);
  }
  void Reframe::rotateYaw(float radians) {
    mat4f rotation;
    rotation.makeRotation(up_, radians);
    forward_ = (rotation * forward_).normalize();
    right_ = forward_.cross(up_);
  }
  void Reframe::rotateAxis(vec3f axis, float radians) {
    mat4f rotation;
    rotation.makeRotation(vec4f(axis), radians);
    forward_ = (rotation * forward_).normalize();
    right_ = (rotation * right_).normalize();
    up_ = right_.cross(forward_);
    right_ = forward_.cross(up_);
  }

  void Reframe::update() {
    combined_ = mat4f(right_, up_, -forward_)
      * mat4f().makeTranslation(translation_);
  }
} // namespace kapusha
