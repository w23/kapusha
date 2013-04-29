// kapusha/ooo
// 2012-2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "Reframe.h"

namespace kapusha {
  class Camera {
  public:
    Camera(vec3f pos = vec3f(1.f, 1.f, 1.f),
           vec3f at = vec3f(0.f, 0.f, 0.f),
           vec3f up = vec3f(0.f, 1.f, 0.f),
           float fov = 60.f, float aspect = 1.f,
           float znear = .1f, float zfar = 100.f);

    inline vec3f position() const { return -frame_.translation(); }
    inline vec3f forward() const { return frame_.forward(); }
    inline vec3f up() const { return frame_.up(); }
    inline vec3f right() const { return frame_.right(); }

    void setProjection(float fov, float aspect, float znear, float zfar);
    void setAspect(float aspect);
    void setAspect(vec2i size);

    void lookAt(vec3f pos, vec3f at, vec3f up = vec3f(0.f, 1.f, 0.f));
    inline void setPosition(vec3f pos) { frame_.setTranslation(-pos); }
    void move(vec3f units);
    void moveForward(float units);
    void moveRigth(float units);
    void moveUp(float units);

    void setAtPosition(vec3f at);
    void setOrientation(vec3f forward = vec3f(0.f, 0.f, -1.f),
                        vec3f up = vec3f(0.f, 1.f, 0.f));
    void rotatePitch(float radians);
    void rotateRoll(float radians);
    void rotateYaw(float radians);
    void rotateAxis(vec3f axis, float radians);

    void update();
    inline const mat4f &getProjection() const { return projection_; }
    inline const mat4f &getView() const { return frame_.getMatrix(); }
    inline const mat4f &getViewProjection() const { return viewProjection_; }

  private:
    Reframe frame_;
    mat4f projection_;
    mat4f viewProjection_;
    float fov_, aspect_, znear_, zfar_;
  };
} // namespace kapusha
