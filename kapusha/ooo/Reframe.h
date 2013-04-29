// kapusha/ooo
// 2012-2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include <kapusha/math.h>

namespace kapusha {
  struct Reframe {
    inline Reframe() = default;
    Reframe(vec3f translation,
            vec3f forward = vec3f(0.f, 0.f, -1.f),
            vec3f up      = vec3f(0.f, 1.f, 0.f));

    inline void setTranslation(vec3f translation) { translation_ = translation; }
    inline void move(vec3f units) { translation_ += units; }
    inline void moveForward(float units) { translation_ += forward_ * units; }
    inline void moveRight(float units) { translation_ += right_ * units; }
    inline void moveUp(float units) { translation_ += up_ * units; }

    void setOrientation(vec3f forward = vec3f(0.f, 0.f, -1.f),
                        vec3f up = vec3f(0.f, 1.f, 0.f));
    void rotatePitch(float radians);
    void rotateRoll(float radians);
    void rotateYaw(float radians);
    void rotateAxis(vec3f axis, float radians);

    inline vec3f translation() const { return translation_; }
    inline vec3f right() const { return right_; }
    inline vec3f up() const { return up_; }
    inline vec3f forward() const { return forward_; }
    
    void update();
    inline const mat4f& getMatrix() const { return combined_; }
  protected:
    vec3f translation_, right_, up_, forward_;
    mat4f combined_;
  }; // struct Reframe
} // namespace kapusha
