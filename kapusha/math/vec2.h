// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include <kapusha/math.h>

namespace kapusha {
  template <typename T> struct vec2 {
    typedef T type;
    T x, y;

    inline vec2() {}
    inline explicit vec2(T v) : x(v), y(v) {}
    inline vec2(T _x, T _y) : x(_x), y(_y) {}
    template <typename R> inline vec2(const vec2<R>& v)
      : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}
    template <typename R> inline vec2& operator=(const vec2<R>& v) {
      x = static_cast<T>(v.x), y = static_cast<T>(v.y); return *this;
    }
    const T *tptr() const { return &x; }

    vec2<T> yx() const { return vec2(y, x); }

    T product() const { return x * y; }
    T dot(vec2 r) const { return x*r.x + y*r.y; }
    T length_sq() const { return dot(*this); }
    T length() const { return sqrt(length_sq()); }
    T rlength() const { return rsqrt(length_sq()); }
    vec2 recip() const { return vec2(::kapusha::recip(x), ::kapusha::recip(y)); }
    vec2 normalized() const { return *this * rlength(); }
    vec2 normalize() { return *this = normalized(); }
    vec2 abs() const { return vec2(abs(x), abs(y)); }
    vec2 clamped(T min, T max) const {
      return vec2(clamp(x, min, max), clamp(y, min, max));
    }
    vec2 clamped(vec2 min, vec2 max) const {
      return vec2(clamp(x, min.x, max.x), clamp(y, min.y, max.y));
    }

    vec2 operator-() const { return vec2(-x, -y); }
    vec2 operator+(const vec2& v) const { return vec2(x+v.x, y+v.y); }    
    vec2 operator-(const vec2& v) const { return vec2(x-v.x, y-v.y); }
    vec2 operator*(const vec2& v) const { return vec2(x*v.x, y*v.y); }
    vec2 operator/(const vec2& v) const { return *this * v.recip(); }
    vec2 operator*(T r) const { return vec2(x*r, y*r); }
    vec2 operator/(T r) const { return *this * ::kapusha::recip(r); }
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
    bool operator<(const vec2<T>& v) const { return length_sq() < v.length_sq(); }
    
  };
    
  template <typename T>
  vec2<T> operator*(T t, const vec2<T>& v)
  {
    return v * t;
  }
} // namespace kapusha
