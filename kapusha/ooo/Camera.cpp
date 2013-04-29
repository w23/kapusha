// kapusha/ooo
// 2012-2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#include "Camera.h"

namespace kapusha {
  Camera::Camera(vec3f pos, vec3f at, vec3f up,
                 float fov, float aspect, float znear, float zfar) {
    setProjection(fov, aspect, znear, zfar);
    lookAt(pos, at, up);
  }
  
  void Camera::setProjection(float fov, float aspect, float znear, float zfar) {
    float f = 1.f / tanf(fov * c_kdeg2rad * .5f);
    projection_ = mat4f(vec4f(f/aspect, 0.f, 0.f, 0.f),
                              vec4f(0.f, f, 0.f, 0.f),
                              vec4f(0.f, 0.f,
                                          (zfar+znear) / (znear - zfar),
                                          2.f * zfar * znear / (znear - zfar)),
                              vec4f(0.f, 0.f, -1.f, 0.f));
    fov_ = fov;
    aspect_ = aspect;
    znear_ = znear;
    zfar_ = zfar;
  }
  void Camera::setAspect(float aspect) {
    if (aspect != aspect_) setProjection(fov_, aspect, znear_, zfar_);
  }
  void Camera::setAspect(vec2i size) {
    setAspect((float)size.x / (float)size.y);
  }

  void Camera::lookAt(vec3f pos, vec3f at, vec3f up) {
    setPosition(pos);
    frame_.setOrientation(at - pos, up);
    update();
  }
  void Camera::move(vec3f units) {
    frame_.move(-units);
  }
  void Camera::moveForward(float units) {
    frame_.moveForward(-units);
  }
  void Camera::moveRigth(float units) {
    frame_.moveRight(-units);
  }
  void Camera::moveUp(float units) {
    frame_.moveUp(-units);
  }

  void Camera::setAtPosition(vec3f at) {
    frame_.setOrientation(at - position(), frame_.up());
  }
  void Camera::setOrientation(vec3f forward, vec3f up) {
    frame_.setOrientation(forward, up);
  }
  void Camera::rotatePitch(float radians) {
    frame_.rotatePitch(radians);
  }
  void Camera::rotateRoll(float radians) {
    frame_.rotateRoll(radians);
  }
  void Camera::rotateYaw(float radians) {
    frame_.rotateYaw(radians);
  }
  void Camera::rotateAxis(vec3f axis, float radians) {
    frame_.rotateAxis(axis, radians);
  }
  
  void Camera::update() {
    frame_.update();
    viewProjection_ = getProjection() * getView();
  }
  
} // namespace kapusha
