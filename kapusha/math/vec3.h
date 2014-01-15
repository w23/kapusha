#pragma once
#include "vec2.h"

namespace kapusha {
namespace math {

template <typename T> struct vec3 {
  T x, y, z;

#if _MSC_VER < 1800
  inline vec3() {}
#else
  inline vec3() = default;
#endif

  inline vec3(T v) : x(v), y(v), z(v) {}
  inline vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
  inline explicit vec3(const T* p) : x(p[0]), y(p[1]), z(p[2]) {}
  inline explicit vec3(const vec2<T>& v, T _z = 0)
    : x(v.x), y(v.y), z(_z) {}
  template <typename U> inline explicit vec3(const vec3<U>& v)
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {}
  template <typename U> inline vec3& operator=(const vec3<U>& v) {
    x = static_cast<T>(v.x), y = static_cast<T>(v.y), z = static_cast<T>(v.z);
    return *this;
  }
  const T *tptr() const { return &x; }

  vec2<T> xx() const { return vec2<T>(x, x); }
  vec2<T> yy() const { return vec2<T>(y, y); }
  vec2<T> zz() const { return vec2<T>(z, z); }
  vec2<T> xy() const { return vec2<T>(x, y); }
  vec2<T> yx() const { return vec2<T>(y, x); }
  vec2<T> xz() const { return vec2<T>(x, z); }
  vec2<T> zx() const { return vec2<T>(z, x); }
  vec2<T> yz() const { return vec2<T>(y, z); }
  vec2<T> zy() const { return vec2<T>(z, y); }

#define SWIZ_(X, Y, Z) inline vec3 X##Y##Z##() const { return vec3(X, Y, Z); }
  SWIZ_(x, x, x) SWIZ_(x, x, y) SWIZ_(x, x, z)
  SWIZ_(x, y, x) SWIZ_(x, y, y) SWIZ_(x, y, z)
  SWIZ_(x, z, x) SWIZ_(x, z, y) SWIZ_(x, z, z)
  SWIZ_(y, x, x) SWIZ_(y, x, y) SWIZ_(y, x, z)
  SWIZ_(y, y, x) SWIZ_(y, y, y) SWIZ_(y, y, z)
  SWIZ_(y, z, x) SWIZ_(y, z, y) SWIZ_(y, z, z)
  SWIZ_(z, x, x) SWIZ_(z, x, y) SWIZ_(z, x, z)
  SWIZ_(z, y, x) SWIZ_(z, y, y) SWIZ_(z, y, z)
  SWIZ_(z, z, x) SWIZ_(z, z, y) SWIZ_(z, z, z)
#undef SWIZ_

  /// \todo to vec4 swizzles

  vec3 &operator=(const T* ptr) { return *this = vec3(ptr[0],ptr[1],ptr[2]); }
  vec3 operator-() const { return vec3(-x, -y, -z); }
  vec3 operator+(const vec3& r) const { return vec3(x+r.x, y+r.y, z+r.z); }
  vec3 operator-(const vec3& r) const { return vec3(x-r.x, y-r.y, z-r.z); }
  vec3 operator*(const vec3& r) const { return vec3(x*r.x, y*r.y, z*r.z); }
  vec3 operator*(T r) const { return vec3(x*r, y*r, z*r); }
  vec3 operator/(const vec3& r) const { return *this * r.recip(); }
  vec3 operator/(T r) const { return *this * ::kapusha::recip(r); }
  vec3 &operator+=(const vec3& r) { return *this = *this + r; }
  vec3 &operator-=(const vec3& r) { return *this = *this - r; }
  vec3 &operator*=(const vec3& r) { return *this = *this * r; }
  vec3 &operator/=(const vec3& r) { return *this = *this / r; }
  bool operator==(const vec3 &r) const {
    return r.x == x && r.y == y && r.z == z;
  }
  bool operator!=(const vec3 &r) const {
    return r.x != x || r.y != y || r.z != z;
  }
}; // vec3

template <typename T> T dot(vec3<T> a, vec3<T> b) {
  return a.x*b.x + a.y*b.y + a.z*b.z;
}
template <typename T> T length2(vec3<T> v) { return dot(v, v); }
template <typename T> T length(vec3<T> v) { return sqrt(length2(v)); }
template <typename T> T rlength(vec3<T> v) { return rsqrt(length2(v)); }
template <typename T> vec3<T> recip(vec3<T> v) {
  return vec3<T>(recip(v.x), recip(v.y), recip(v.z));
}
template <typename T> vec3<T> normalize(vec3<T> v) { return v * rlength(v); }
template <typename T> vec3<T> cross(vec3<T> a, vec3<T> b) {
  return vec3<T>(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}
template <typename T> vec3<T> clamp(vec3<T> v, T min, T max) {
  return vec3<T>(
    clamp(v.x, min, max), clamp(v.y, min, max), clamp(v.z, min, max));
}
template <typename T> vec3<T> clamp(vec3<T> v, vec3<T> min, vec3<T> max) {
  return vec3<T>(
    clamp(v.x, min.x, max.x),
    clamp(v.y, min.y, max.y),
    clamp(v.z, min.z, max.z));
}

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
template <typename T> T min(vec3<T> v) { return min(v.x, min(v.y, v.z)); }
template <typename T> T max(vec3<T> v) { return max(v.x, max(v.y, v.z)); }
template <typename T> vec3<T> abs(vec3<T> v) {
  return vec3(abs(v.x), abs(v.y), abs(v.z));
}
template <typename T> vec3<T> mod(vec3<T> v, vec3<T> d) {
  return vec3<T>(mod(v.x, d.x), mod(v.y, d.y), mod(v.z, d.z));
}

} // namespace math
} // namespace kapusha
