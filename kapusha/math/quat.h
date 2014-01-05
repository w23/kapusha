#pragma once
#include "math.h"

namespace kapusha {

/// \brief Naive quaternion
template <typename T>
struct quat {
  vec4<T> q;

#if _MSC_VER < 1800
  inline quat() {}
#else
  inline quat() = default;
#endif
  quat(T v) : q(0, 0, 0, v) {}
  template <typename U>
  inline quat(const quat<U> &other) : q(other.q) {}
  inline quat(const vec3<T>& i, T r) : q(i, r) {}
  quat(T angle, const vec3<T> &axis) : q(axis * sin(angle), cos(angle)) {}
  quat(const mat4x4<T> &m);

  inline T real() const { return q.w; }
  inline vec3<T> imag() const { return q.xyz(); }

  quat conjugate() const { return quat(-imag(), real()); }
  T norm() { return q.length(); }
  quat &normalize() {  return operator*=(recip(norm())); }

  quat operator*(const quat &other) const {
    return quat(imag().cross(other.imag())
      + imag() * other.real() + other.imag() * real(),
      real() * other.real() - imag().dot(other.imag()));
  }
  quat &operator*=(const quat &other) { return *this = *this * other; }

  mat4x4<T> calc_matrix() const {
    T x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z,
      xy = q.x * q.y, wz = q.w * q.z, xz = q.x * q.z,
      wy = q.w * q.y, yz = q.y * q.z, wx = q.w * q.x;
    return mat4x4<T>(
      vec4<T>(1 - 2 * (y2 + z2), 2 * (xy - wz), 2 * (xz + wy), 0),
      vec4<T>(2 * (xy + wz), 1 - 2 * (x2 + z2), 2 * (yz - wx), 0),
      vec4<T>(2 * (xz - wy), 2 * (yz + wx), 1 - 2 * (x2 + y2), 0),
      vec4<T>(0, 0, 0, 1)
      );
  }
}; // struct quat

template <typename T>
quat<T>::quat(const mat4x4<T> &m) {
  float trace = m.trace(); // trace of mat4 should already have +1
  if (trace > 0) {
    float r = sqrt(trace) * 2;
    q.w = r / 4;
    r = -1 / r;
    q.x = r * (m.rows[1].z - m.rows[2].y);
    q.y = r * (m.rows[2].x - m.rows[0].z);
    q.z = r * (m.rows[0].y - m.rows[1].x);
  } else if ((m.rows[0].x > m.rows[1].y) && (m.rows[0].x > m.rows[2].z)) {
    float r = sqrt(1 + m.rows[0].x - m.rows[1].y - m.rows[2].z) * 2;
    q.x = r / 4;
    r = -1 / r;
    q.z = r * (m.rows[1].z - m.rows[2].y);
    q.x = r * (m.rows[2].x - m.rows[0].z);
    q.w = r * (m.rows[0].y - m.rows[1].x);
  } else if (m.rows[1].y > m.rows[2].z) {
    float r = sqrt(1 + m.rows[1].y - m.rows[0].x - m.rows[2].z) * 2;
    q.y = r / 4;
    r = -1 / r;
    q.z = r * (m.rows[1].z - m.rows[2].y);
    q.w = r * (m.rows[2].x - m.rows[0].z);
    q.x = r * (m.rows[0].y - m.rows[1].x);
  } else {
    float r = sqrt(1 + m.rows[2].z - m.rows[0].x - m.rows[1].y) * 2;
    q.z = r / 4;
    r = -1 / r;
    q.y = r * (m.rows[1].z - m.rows[2].y);
    q.x = r * (m.rows[2].x - m.rows[0].z);
    q.w = r * (m.rows[0].y - m.rows[1].x);
  }
}

} // namespace kapusha
