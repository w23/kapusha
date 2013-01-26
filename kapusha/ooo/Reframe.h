#pragma once

#include "../core/Shared.h"
#include "../math/types.h"

namespace kapusha {

  class Reframe : public Shared
  {
  public:
    Reframe(vec3f pos     = vec3f(0.f, 0.f, 0.f),
            vec3f forward = vec3f(0.f, 0.f, 1.f),
            vec3f up      = vec3f(0.f, 1.f, 0.f));
    
    inline void translate(vec3f translation) {
      translation_ = vec4f(translation);
    }
    
    inline void move(vec3f units) {
      translation_ += vec4f(units);
    }
    
    void orient(vec3f forward, vec3f up);
    
    void lookAt(vec3f pos,
                vec3f at = vec3f(0.f, 0.f, 0.f),
                vec3f up = vec3f(0.f, 1.f, 0.f));
    
    void moveForward(float units);
    void moveRigth(float units);
    void moveUp(float units);
    void rotatePitch(float radians);
    void rotateRoll(float radians);
    void rotateYaw(float radians);
	void rotateAxis(vec3f axis, float radians);
    
    void update();
    
    inline vec3f translation() const { return translation_.xyz(); }
    inline vec3f right() const { return right_.xyz(); }
    inline vec3f up() const { return up_.xyz(); }
    inline vec3f forward() const { return forward_.xyz(); }
    inline const mat4f& getMatrix() const { return combined_; }
    
  private:
    vec4f translation_, right_, up_, forward_;
    mat4f combined_;
  };
  
} // namespace kapusha