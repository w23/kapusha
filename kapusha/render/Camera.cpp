#include "Camera.h"

namespace kapusha {
  Camera::Camera(vec3f pos, vec3f at, vec3f up,
                 float fov, float aspect, float znear, float zfar)
  {
    lookAt(pos, at, up);
    setProjection(fov, aspect, znear, zfar);
  }

  Camera::~Camera()
  {
  }
  
  void Camera::setProjection(float fov, float aspect, float znear, float zfar)
  {
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
  
  void Camera::setAspect(float aspect)
  {
    if (aspect != aspect_)
      setProjection(fov_, aspect, znear_, zfar_);
  }
  
  void Camera::lookAt(vec3f pos, vec3f at, vec3f up)
  {
    position_ = pos;
    forward_ = (at - pos).normalize();
    right_ = (forward_ ^ up).normalize();
    up_ = right_ ^ forward_;
    need_update_ = true;
  }
  
  void Camera::moveForward(float units)
  {
    position_ += forward_ * units;
    need_update_ = true;
  }
  
  void Camera::moveRigth(float units)
  {
    position_ += right_ * units;
    need_update_ = true;
  }
  
  void Camera::moveUp(float units)
  {
    position_ += up_ * units;
    need_update_ = true;
  }
  
  void Camera::rotatePitch(float radians)
  {
    mat4f rotation;
    rotation.rotationAroundAxis(right_, radians);
    forward_ = (rotation * forward_).normalize();
    up_ = right_ ^ forward_;
    need_update_ = true;
  }
  
  void Camera::rotateRoll(float radians)
  {
    mat4f rotation;
    rotation.rotationAroundAxis(forward_, radians);
    up_ = (rotation * up_).normalize();
    right_ = forward_ ^ up_;
    need_update_ = true;
  }
  
  void Camera::rotateYaw(float radians)
  {
    mat4f rotation;
    rotation.rotationAroundAxis(up_, radians);
    forward_ = (rotation * forward_).normalize();
    right_ = forward_ ^ up_;
    need_update_ = true;
  }

  void Camera::rotateAxis(vec3f axis, float radians)
  {
    mat4f rotation;
    rotation.rotationAroundAxis(axis, radians);
    forward_ = (rotation * forward_).normalize();
    right_ = (rotation * right_).normalize();
    up_ = right_ ^ forward_;
	right_ = forward_ ^ up_;
    need_update_ = true;
  }
  
  bool Camera::update()
  {
    if (!need_update_)
      return false;
    
    view_ = mat4f(vec4f(right_, 0.f),
                        vec4f(up_, 0.f),
                        vec4f(-forward_, 0.f))
      * mat4f().translation(vec4f(position_, 0.f));
    
    need_update_ = false;
    return true;
  }
} // namespace kapusha
