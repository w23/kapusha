#pragma once

#include "../math/types.h"

namespace kapusha {

  class Camera {
  public:
    Camera(math::vec3f pos = math::vec3f(1.f, 1.f, 1.f),
           math::vec3f at = math::vec3f(0.f, 0.f, 0.f),
           math::vec3f up = math::vec3f(0.f, 1.f, 0.f),
           float fov = 90.f, float aspect = 16.f / 9.f,
           float znear = .01f, float zfar = 100.f);
    ~Camera();

    void setProjection(float fov, float aspect, float znear, float zfar);
    void setAspect(float aspect);
    
    void lookAt(math::vec3f pos, math::vec3f at,
                math::vec3f up = math::vec3f(0.f, 1.f, 0.f));

    void moveForward(float units);
    void moveRigth(float units);
    void moveUp(float units);
    void rotatePitch(float radians);
    void rotateRoll(float radians);
    void rotateYaw(float radians);
    
    bool update();

    const math::mat4f& getView() const { return view_; }
    const math::mat4f& getProjection() const { return projection_; }

  private:
    float fov_, aspect_, znear_, zfar_;
    math::vec3f position_;
    math::vec3f forward_;
    math::vec3f right_;
    math::vec3f up_;
    bool need_update_;

    math::mat4f view_;
    math::mat4f projection_;
  };

} // namespace kapusha