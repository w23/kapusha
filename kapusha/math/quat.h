#pragma once
#include "math.h"

namespace kapusha {
namespace math {

/// \brief Naive quaternion
template <typename T> struct quat {
  vec4<T> q;

#if _MSC_VER < 1800
  inline quat() {}
#else
  inline quat() = default;
#endif
  quat(T v) : q(0, 0, 0, v) {}
  template <typename U>
  inline quat(const quat<U> &other) : q(other.q) {}
  inline explicit quat(const vec4<T>& v) : q(v) {}
  inline quat(const vec3<T>& i, T r) : q(i, r) {}
  quat(T angle, const vec3<T> &axis) : q(axis * sin(angle), cos(angle)) {}
  quat(const mat4<T> &m);

  inline const vec4<T> *operator->() const { return &q; }

  inline T real() const { return q.w; }
  inline vec3<T> imag() const { return q.xyz(); }

  quat operator*(const quat &other) const {
    return quat(cross(imag(), other.imag())
      + imag() * other.real() + other.imag() * real(),
      real() * other.real() - dot(imag(), other.imag()));
  }
  quat &operator*=(const quat &other) { return *this = *this * other; }
}; // struct quat

template <typename T> quat<T>::quat(const mat4<T> &m) {
  T tr = m.m00() + m.m11() + m.m22();
  if (tr > T(0)) {
    const T root = sqrt(tr + T(1));
    const T rcp = T(1) / (root * T(2));
    q.x = rcp * (m.m12() - m.m21());
    q.y = rcp * (m.m20() - m.m02());
    q.z = rcp * (m.m01() - m.m10());
    q.w = root / T(2);
    return;
  }

  tr = m.m00() - m.m11() - m.m22();
  if (tr > T(0)) {
    const T root = sqrt(tr + T(1));
    const T rcp = T(1) / (root * T(2));
    q.x = root / T(2);
    q.y = rcp * (m.m10() + m.m01());
    q.z = rcp * (m.m20() + m.m02());
    q.w = rcp * (m.m12() - m.m21());
    return;
  }

  tr = - m.m00() + m.m11() - m.m22();
  if (tr > T(0)) {
    const T root = sqrt(tr + T(1));
    const T rcp = T(1) / (root * T(2));
    q.x = rcp * (m.m10() + m.m01());
    q.y = root / T(2);
    q.z = rcp * (m.m21() + m.m12());
    q.w = rcp * (m.m20() - m.m02());
    return;
  }

  tr = - m.m00() - m.m11() + m.m22();
  if (tr > T(0)) {
    const T root = sqrt(tr + T(1));
    const T rcp = T(1) / (root * T(2));
    q.x = rcp * (m.m02() + m.m20());
    q.y = rcp * (m.m21() + m.m12());
    q.z = root / T(2);
    q.w = rcp * (m.m02() - m.m10());
    return;
  }

  //KP_ASSERT(!"Unsolvable matrix");
}

template <typename T> quat<T> conjugate(quat<T> q) { return quat<T>(-q.imag(), q.real()); }
template <typename T> T norm(quat<T> q) { return length(q.q); }
template <typename T> quat<T> normalize(quat<T> q) { return quat<T>(q.q *= recip(norm(q))); }
template <typename T> mat4<T> mat4_rotation(quat<T> q) {
  const T
    x2 = q->x * q->x, y2 = q->y * q->y, z2 = q->z * q->z,
    xy = q->x * q->y, wz = q->w * q->z, xz = q->x * q->z,
    wy = q->w * q->y, yz = q->y * q->z, wx = q->w * q->x;
  return mat4<T>(
    vec4<T>(T(1) - T(2) * (y2 + z2), T(2) * (xy - wz), T(2) * (xz + wy), 0),
    vec4<T>(T(2) * (xy + wz), T(1) - T(2) * (x2 + z2), T(2) * (yz - wx), 0),
    vec4<T>(T(2) * (xz - wy), T(2) * (yz + wx), T(1) - T(2) * (x2 + y2), 0),
    vec4<T>(0, 0, 0, 1));
}

} // namespace math
} // namespace kapusha
