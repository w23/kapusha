// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru

#include "types.h"

namespace math {
  
  const float c_pi = 3.14159265358979323844f;
  const float c_2pi = c_pi * 2.f;
  const float c_pi2 = c_pi * .5f;
  const float c_pi4 = c_pi * .25f;
  const float c_kdeg2rad = c_pi / 180.f;
  const float c_krad2deg = 180.f / c_pi;


  template<>
  mat4f& mat4f::rotationAroundAxis(vec3f axis, float angle)
  {
    float s = sinf(angle), c = cosf(angle);
    *this = mat4f(c) + 
      mat4f(vec4f(0.f, -axis.z, axis.y),
            vec4f(axis.z, 0.f, -axis.y),
            vec4f(-axis.y, axis.x, 0.f)) * s +
      mat4f(vec4f(axis) * axis.x,
            vec4f(axis) * axis.y,
            vec4f(axis) * axis.z) * (1.f - c);
    m[12] = m[13] = m[14] = 0.f;
    m[15] = 1.f;
    return *this;
  }

} // namespace math