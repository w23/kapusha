#pragma once

#include "../math/types.h"

namespace kapusha {

  class Camera {
  public:
    Camera(vec3f pos = vec3f(1.f, 1.f, 1.f),
           vec3f at = vec3f(0.f, 0.f, 0.f),
           vec3f up = vec3f(0.f, 1.f, 0.f),
           float fov = 90.f, float aspect = 16.f / 9.f,
           float znear = .01f, float zfar = 100.f);
    ~Camera();

    void setProjection(float fov, float aspect, float znear, float zfar);
    void setAspect(float aspect);
    
    void lookAt(vec3f pos, vec3f at,
                vec3f up = vec3f(0.f, 1.f, 0.f));
    
    void moveForward(float units);
    void moveRigth(float units);
    void moveUp(float units);
    void rotatePitch(float radians);
    void rotateRoll(float radians);
    void rotateYaw(float radians);
	void rotateAxis(vec3f axis, float radians);
    
    bool update();

    const mat4f& getView() const { return view_; }
    const mat4f& getProjection() const { return projection_; }

  private:
    float fov_, aspect_, znear_, zfar_;
    vec3f position_;
    vec3f forward_;
    vec3f right_;
    vec3f up_;
    bool need_update_;

    mat4f view_;
    mat4f projection_;
  };

} // namespace kapusha
