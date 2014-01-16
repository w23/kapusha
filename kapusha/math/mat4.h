// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "vec4.h"

namespace kapusha {
namespace math {
template <typename T> struct mat4 {
  vec4<T> rows[4];

#if _MSC_VER < 1800
  inline mat4() {}
#else
  inline mat4() = default;
#endif
  inline mat4(T v) {
    rows[0] = vec4<T>(v, 0, 0, 0);
    rows[1] = vec4<T>(0, v, 0, 0);
    rows[2] = vec4<T>(0, 0, v, 0);
    rows[3] = vec4<T>(0, 0, 0, 1);
  }
  inline mat4(const vec4<T>& r0, const vec4<T>& r1, const vec4<T>& r2,
                const vec4<T>& r3 = vec4<T>(0,0,0,1)) {
    rows[0] = r0; rows[1] = r1; rows[2] = r2; rows[3] = r3;
  }
  inline explicit mat4(const T *p) {
    rows[0] = vec4<T>(p);
    rows[1] = vec4<T>(p+4);
    rows[2] = vec4<T>(p+8);
    rows[3] = vec4<T>(p+12);
  }
  const T *tptr() const { return rows[0].tptr(); }

#define M4MCR(C,R,r,c) \
  inline T m ## R ## C() const { return rows[r].c; } \
  inline T &m ## R ## C() { return rows[r].c; }
  M4MCR(1,1,0,x) M4MCR(2,1,0,y) M4MCR(3,1,0,z) M4MCR(4,1,0,w)
  M4MCR(1,2,1,x) M4MCR(2,2,1,y) M4MCR(3,2,1,z) M4MCR(4,2,1,w)
  M4MCR(1,3,2,x) M4MCR(2,3,2,y) M4MCR(3,3,2,z) M4MCR(4,3,2,w)
  M4MCR(1,4,3,x) M4MCR(2,4,3,y) M4MCR(3,4,3,z) M4MCR(4,4,3,w)
#undef M4MCR

  mat4 operator*(T r) const {
    return mat4(rows[0]*r, rows[1]*r, rows[2]*r, rows[3]*r);
  }
  vec4<T> operator*(const vec4<T>& r) const {
    return vec4<T>(rows[0].dot(r), rows[1].dot(r), rows[2].dot(r), rows[3].dot(r));
  }
  vec3<T> operator*(const vec3<T>& r) const { return (*this * vec4<T>(r)).xyz(); }
  mat4 operator*(const mat4& r) const {
    mat4 rt(transpose(r));
#define RRTR(i,j) dot(rows[i], rt.rows[j])
    return mat4(
      vec4<T>(RRTR(0,0), RRTR(0,1), RRTR(0,2), RRTR(0,3)),
      vec4<T>(RRTR(1,0), RRTR(1,1), RRTR(1,2), RRTR(1,3)),
      vec4<T>(RRTR(2,0), RRTR(2,1), RRTR(2,2), RRTR(2,3)),
      vec4<T>(RRTR(3,0), RRTR(3,1), RRTR(3,2), RRTR(3,3)));
#undef RRTR
  }
}; // mat4

template <typename T> mat4<T> transpose(const mat4<T> &m) {
  return mat4<T>(
    vec4<T>(m.rows[0].x, m.rows[1].x, m.rows[2].x, m.rows[3].x),
    vec4<T>(m.rows[0].y, m.rows[1].y, m.rows[2].y, m.rows[3].y),
    vec4<T>(m.rows[0].z, m.rows[1].z, m.rows[2].z, m.rows[3].z),
    vec4<T>(m.rows[0].w, m.rows[1].w, m.rows[2].w, m.rows[3].w));
}

template <typename T> T trace(const mat4<T> &m) {
  return m.rows[0].x + m.rows[1].y + m.rows[2].z + m.rows[3].w;
}

  /// \todo T det(const mat4<T> &m)

template <typename T> mat4<T> mat4_scale(vec3<T> scale) {
  return mat4<T>(
    vec4<T>(scale.x, 0, 0, 0),
    vec4<T>(0, scale.y, 0, 0),
    vec4<T>(0, 0, scale.z, 0),
    vec4<T>(0, 0, 0, 1));
}
template <typename T> mat4<T> mat4_translation(vec3<T> translation) {
  return mat4<T>(
    vec4<T>(1, 0, 0, translation.x),
    vec4<T>(0, 1, 0, translation.y),
    vec4<T>(0, 0, 1, translation.z),
    vec4<T>(0, 0, 0, 1));
}
template <typename T> mat4<T> mat4_rotation(vec3<T> axis, T angle) {
  const T s = sin(angle), c = cos(angle), c1 = T(1) - c;
  const vec3<T> a2 = axis * axis;
  const T xy = axis.x * axis.y;
  const T xz = axis.x * axis.z;
  const T yz = axis.y * axis.z;
  return mat4<T>(
    vec4<T>(c1*a2.x + c, c1*xy - s*axis.z, c1*xz + s*axis.y, 0),
    vec4<T>(c1*xy + s*axis.z, c1 * a2.y+c, c1*yz - s*axis.x, 0),
    vec4<T>(c1*xz - s*axis.y, c1*yz + s*axis.x, c1*a2.z + c, 0),
    vec4<T>( 0, 0, 0, 1));
}

} // namespace math
} // namespace kapusha
