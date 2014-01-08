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
  inline quat(const vec4<T>& v) : q(v) {}
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
  const T tr = trace(m); // trace of mat4 should already have +1
  if (tr > T(0)) {
    T r = sqrt(tr) * T(2);
    q.w = r / T(4);
    r = T(-1) / r;
    q.x = r * (m.rows[1].z - m.rows[2].y);
    q.y = r * (m.rows[2].x - m.rows[0].z);
    q.z = r * (m.rows[0].y - m.rows[1].x);
  } else if ((m.rows[0].x > m.rows[1].y) && (m.rows[0].x > m.rows[2].z)) {
    T r = sqrt(T(1) + m.rows[0].x - m.rows[1].y - m.rows[2].z) * T(2);
    q.x = r / T(4);
    r = T(-1) / r;
    q.z = r * (m.rows[1].z - m.rows[2].y);
    q.x = r * (m.rows[2].x - m.rows[0].z);
    q.w = r * (m.rows[0].y - m.rows[1].x);
  } else if (m.rows[1].y > m.rows[2].z) {
    T r = sqrt(T(1) + m.rows[1].y - m.rows[0].x - m.rows[2].z) * T(2);
    q.y = r / T(4);
    r = T(-1) / r;
    q.z = r * (m.rows[1].z - m.rows[2].y);
    q.w = r * (m.rows[2].x - m.rows[0].z);
    q.x = r * (m.rows[0].y - m.rows[1].x);
  } else {
    T r = sqrt(T(1) + m.rows[2].z - m.rows[0].x - m.rows[1].y) * T(2);
    q.z = r / T(4);
    r = T(-1) / r;
    q.y = r * (m.rows[1].z - m.rows[2].y);
    q.x = r * (m.rows[2].x - m.rows[0].z);
    q.w = r * (m.rows[0].y - m.rows[1].x);
  }
}

template <typename T> quat<T> conjugate() { return quat<T>(-q.imag(), q.real()); }
template <typename T> T norm(quat<T> q) { return length(q.q); }
template <typename T> quat<T> normalize(quat<T> q) { return quat<T>(q.q *= recip(norm(q))); }
template <typename T> mat4<T> mat4_rotation(quat<T> q) {
  const T
    x2 = q->x * q->x, y2 = q->y * q->y, z2 = q->z * q->z,
    xy = q->x * q->y, wz = q->w * q->z, xz = q->x * q->z,
    wy = q->w * q->y, yz = q->y * q->z, wx = q->w * q->x;
  return mat4<T>(
    vec4<T>(1 - 2 * (y2 + z2), 2 * (xy - wz), 2 * (xz + wy), 0),
    vec4<T>(2 * (xy + wz), 1 - 2 * (x2 + z2), 2 * (yz - wx), 0),
    vec4<T>(2 * (xz - wy), 2 * (yz + wx), 1 - 2 * (x2 + y2), 0),
    vec4<T>(0, 0, 0, 1));
}

} // namespace math
} // namespace kapusha
