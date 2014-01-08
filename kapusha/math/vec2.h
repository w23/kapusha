#pragma once

namespace kapusha {
namespace math {

template <typename T> struct vec2 {
  typedef T type;
  T x, y;

#if !defined(_MSC_VER)
  inline vec2() = default;
#else
  inline vec2() {}
#endif

  inline explicit vec2(T v) : x(v), y(v) {}
  inline vec2(T _x, T _y) : x(_x), y(_y) {}
  template <typename R> inline explicit vec2(const vec2<R>& v)
    : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}
  template <typename R> inline vec2& operator=(const vec2<R>& v) {
    x = static_cast<T>(v.x), y = static_cast<T>(v.y); return *this;
  }
  const T *tptr() const { return &x; }

  vec2<T> xx() const { return vec2<T>(x, x); }
  vec2<T> yy() const { return vec2<T>(y, y); }
  vec2<T> xy() const { return vec2<T>(x, y); }
  vec2<T> yx() const { return vec2<T>(y, x); }
  
  /// \todo to vec3/vec4 swizzles

  vec2 operator-() const { return vec2(-x, -y); }
  vec2 operator+(const vec2& v) const { return vec2(x+v.x, y+v.y); }
  vec2 operator-(const vec2& v) const { return vec2(x-v.x, y-v.y); }
  vec2 operator*(const vec2& v) const { return vec2(x*v.x, y*v.y); }
  vec2 operator/(const vec2& v) const { return *this * recip(v); }
  vec2 operator*(T r) const { return vec2(x*r, y*r); }
  vec2 operator/(T r) const { return *this * recip(r); }
  vec2& operator+=(const vec2& v) { return *this = *this + v; }
  vec2& operator-=(const vec2& v) { return *this = *this - v; }
  vec2& operator*=(const vec2& v) { return *this = *this * v; }
  vec2& operator*=(T v) { return *this = *this * v; }
  vec2& operator/=(T v) { return *this = *this / v; }
  template <typename R> vec2 operator*(const vec2<R>& v) const {
    return vec2(x*v.x, y*v.y);
  }
  template <typename R> vec2 operator/(const vec2<R>& v) const {
    return vec2(x/v.x, y/v.y);
  }
  bool operator==(const vec2<T>& v) const { return v.x == x && v.y == y; } 
  bool operator!=(const vec2<T>& v) const { return v.x != x || v.y != y; }
  // ??? bool operator<(const vec2<T>& v) const { return length2(*this) < length2(v); }
};

template <typename T> vec2<T> operator*(T t, const vec2<T>& v) {
  return v * t;
}

template <typename T> T area(vec2<T> v) { return v.x * v.y; }
template <typename T> T dot(vec2<T> a, vec2<T> b) { return a.x*b.x + a.y*b.y; }
template <typename T> T length2(vec2<T> v) { return dot(v, v); }
template <typename T> T length(vec2<T> v) { return sqrt(length2(v)); }
template <typename T> T rlength(vec2<T> v) { return rsqrt(length2(v)); }
template <typename T> vec2<T> recip(vec2<T> v) {
  return vec2<T>(recip(v.x), recip(v.y));
}
template <typename T> vec2<T> normalize(vec2<T> v) { return v * rlength(v); }
template <typename T> vec2<T> abs(vec2<T> v) {
  return vec2<T>(abs(v.x), abs(v.y));
}
template <typename T> vec2<T> clamp(vec2<T> v, T min, T max) {
  return vec2<T>(clamp(v.x, min, max), clamp(v.y, min, max));
}
template <typename T> vec2<T> clamp(vec2<T> v, vec2<T> min, vec2<T> max) {
  return vec2<T>(clamp(v.x, min.x, max.x), clamp(v.y, min.y, max.y));
}
template <typename T> vec2<T> floor(vec2<T> v) {
  return vec2<T>(floor(v.x), floor(v.y));
}
template <typename T> vec2<T> fract(vec2<T> v) {
  return vec2<T>(fract(v.x), fract(v.y));
}

} // namespace math
} // namespace kapusha
