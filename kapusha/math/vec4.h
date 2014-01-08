#pragma once
#include "vec3.h"

namespace kapusha {
namespace math {

template <typename T> struct vec4 {
  T x, y, z, w;

#if _MSC_VER < 1800
  inline vec4() {}
#else
  inline vec4() = default;
#endif

  inline vec4(T v) : x(v), y(v), z(v), w(v) {}
  inline vec4(T _x, T _y, T _z = 0, T _w = 0)
    : x(_x), y(_y), z(_z), w(_w) {}
  inline explicit vec4(const vec2<T> &v, T _z = 0, T _w = 0)
    : x(v.x), y(v.y), z(_z), w(_w) {}
  inline explicit vec4(const vec3<T> &v, T _w = 0)
    : x(v.x), y(v.y), z(v.z), w(_w) {}
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

#define SWIZ_(X, Y, Z) inline vec3<T> X##Y##Z##() const { return vec3<T>(X, Y, Z); }
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

  inline bool is_direction() const { return w == 0; }

  vec4 operator-() const { return vec4(-x, -y, -z, -w); }
  vec4 operator*(T r) const { return vec4(x*r, y*r, z*r, w*r); }
  vec4 operator/(T r) const { return *this * recip(r); }
  vec4 &operator*=(T r) { return *this = *this * r; }
  vec4 &operator/=(T r) { return *this = *this / r; }
  vec4 operator+(const vec4 &r) const {
    return vec4(x+r.x, y+r.y, z+r.z, w+r.w);
  }
  vec4 operator-(const vec4 &r) const {
    return vec4<T>(x-r.x, y-r.y, z-r.z, w-r.w);
  }
  vec4 operator*(const vec4 &r) const {
    return vec4<T>(x*r.x, y*r.y, z*r.z, w*r.w);
  }
  vec4 operator/(const vec4 &r) const { return *this * recip(r); }
  vec4 &operator+=(const vec4 &r) { return *this = *this + r; }
  vec4 &operator*=(const vec4 &r) { return *this = *this + r; }
}; // vec4

template <typename T> T dot(vec4<T> a, vec4<T> b) {
  return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}
template <typename T> T length2(vec4<T> v) { return dot(v, v); }
template <typename T> T length(vec4<T> v) { return sqrt(length2(v)); }
template <typename T> T rlength(vec4<T> v) { return rsqrt(length2(v)); }
template <typename T> vec4<T> recip(vec4<T> v) {
  return vec4<T>(recip(v.x), recip(v.y), recip(v.z), recip(v.w));
}
template <typename T> vec4<T> normalize(vec4<T> v) { return v * rlength(v); }
template <typename T> vec4<T> cross(vec4<T> a, vec4<T> b) {
    return vec4<T>(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x, 0);
  }

} // namespace math
} // namespace kapusha
