#pragma once

#include "../math/types.h"

namespace kapusha {

  class Camera {
  public:
    Camera();
    ~Camera();

    void setProjection(float fov, float aspect, float znear, float zfar);

    void moveForward(float units);
    void moveRigth(float units);
    void moveUp(float units);
    void rotatePitch(float radians);
    void rotateRoll(float radians);
    void rotateYaw(float radians);

    const math::mat4f& getView() const;
    const math::mat4f& getProjection() const;

  private:
    void update();
    math::vec3f position_;
    math::vec3f forward_;
    math::vec3f right_;

    math::mat4f view_;
    math::mat4f projection_;
  };

} // namespace kapusha