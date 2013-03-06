#pragma once
#include "Reframe.h"

namespace kapusha {
  class Camera : public Reframe {
  public:
    Camera(vec3f pos = vec3f(1.f, 1.f, 1.f),
           vec3f at = vec3f(0.f, 0.f, 0.f),
           vec3f up = vec3f(0.f, 1.f, 0.f),
           float fov = 90.f, float aspect = 16.f / 9.f,
           float znear = .01f, float zfar = 100.f);

    void setProjection(float fov, float aspect, float znear, float zfar);
    void setAspect(float aspect);
    void update();

    inline const mat4f &getProjection() const { return projection_; }
    inline const mat4f &getMvp() const { return mvp_; }

  private:
    float fov_, aspect_, znear_, zfar_;
    mat4f projection_;
    mat4f mvp_;
  };

} // namespace kapusha
