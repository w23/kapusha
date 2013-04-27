#pragma once
#include "../math/types.h"

namespace kapusha {
  struct Reframe {
  public:
    Reframe() = default;
    Reframe(vec3f pos,
            vec3f forward = vec3f(0.f, 0.f, -1.f),
            vec3f up      = vec3f(0.f, 1.f, 0.f));
    void lookAt(vec3f pos,
                vec3f at = vec3f(0.f, 0.f, 0.f),
                vec3f up = vec3f(0.f, 1.f, 0.f));
    inline void translate(vec3f translation) {
      translation_ = vec4f(translation);
    }
    inline void move(vec3f units) {
      translation_ += vec4f(units);
    }
    inline mat4f &matrix() { return combined_; }
    void orient(vec3f forward, vec3f up);
    void moveForward(float units);
    void moveRigth(float units);
    void moveUp(float units);
    void rotatePitch(float radians);
    void rotateRoll(float radians);
    void rotateYaw(float radians);
    void rotateAxis(vec3f axis, float radians);
    void update();
    //! \todo reverseUpdate(); //! recalc vectors from combined matrix
    inline vec3f translation() const { return translation_.xyz(); }
    inline vec4f& translation() { return translation_; }
    inline vec3f right() const { return right_.xyz(); }
    inline vec3f up() const { return up_.xyz(); }
    inline vec3f forward() const { return forward_.xyz(); }
    inline const mat4f& getMatrix() const { return combined_; }
  protected:
    vec4f translation_, right_, up_, forward_;
    mat4f combined_;
  };
} // namespace kapusha
