// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#include <cstdlib>
#include "types.h"

namespace kapusha {
  const float c_pi = 3.14159265358979323844f;
  const float c_2pi = c_pi * 2.f;
  const float c_pi2 = c_pi * .5f;
  const float c_pi4 = c_pi * .25f;
  const float c_kdeg2rad = c_pi / 180.f;
  const float c_krad2deg = 180.f / c_pi;

  template<> mat4f& mat4f::make_rotation(vec3f axis, float angle) {
    const float s = sinf(angle), c = cosf(angle), c1 = 1.f - c;
    const vec3f a2 = axis * axis;
    const float xy = axis.x * axis.y;
    const float xz = axis.x * axis.z;
    const float yz = axis.y * axis.z;
    rows[0] = vec4f(c1*a2.x + c, c1*xy - s*axis.z, c1*xz + s*axis.y, 0);
    rows[1] = vec4f(c1*xy + s*axis.z, c1 * a2.y+c, c1*yz - s*axis.x, 0);
    rows[2] = vec4f(c1*xz - s*axis.y, c1*yz + s*axis.x, c1*a2.z + c, 0);
    rows[3] = vec4f( 0, 0, 0, 1);
    return *this;
  }
  
  float frand() { return (float)rand() / RAND_MAX; }
  float frand(float min, float max) { return min + frand() * (max - min); }
} // namespace kapusha
