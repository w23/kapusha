// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru

#include <stdlib.h>
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
    float s = sinf(angle), c = cosf(angle), c1 = 1.f - c;
    vec3f a2 = axis * axis;
    float xy = axis.x * axis.y;
    float xz = axis.x * axis.z;
    float yz = axis.y * axis.z;
    m[0]  = c1 * a2.x + c;
    m[1]  = c1 * xy   + s * axis.z;
    m[2]  = c1 * xz   - s * axis.y;
    m[3]  = 0.f;
    m[4]  = c1 * xy   - s * axis.z;
    m[5]  = c1 * a2.y + c;
    m[6]  = c1 * yz   + s * axis.x;
    m[7]  = 0.f;
    m[8]  = c1 * xz   + s * axis.y;
    m[9]  = c1 * yz   - s * axis.x;
    m[10] = c1 * a2.z + c;
    m[11] = 0.f;
    m[12] = m[13] = m[14] = 0.f;
    m[15] = 1.f;
    return *this;
  }
  
  float frand()
  {
    return (float)rand() / RAND_MAX;
  }
  
  float frand(float min, float max)
  {
    return min + frand() * (max - min);
  }

} // namespace math