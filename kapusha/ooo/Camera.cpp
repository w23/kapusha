#include "Camera.h"

namespace kapusha {
  Camera::Camera(vec3f pos, vec3f at, vec3f up,
                 float fov, float aspect, float znear, float zfar) {
    lookAt(pos, at, up);
    setProjection(fov, aspect, znear, zfar);
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
  
  void Camera::update() {
    Reframe::update();
    mvp_ = projection_ * getMatrix();
  }
  
} // namespace kapusha
