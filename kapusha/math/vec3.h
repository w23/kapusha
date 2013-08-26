// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include <kapusha/math.h>
#include "vec2.h"

namespace kapusha {
  template <typename T> struct vec3 {
    T x, y, z;

    inline vec3() {}
    inline vec3(T v) : x(v), y(v), z(v) {}
    inline vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    inline vec3(const T* p) : x(p[0]), y(p[1]), z(p[2]) {}
    inline explicit vec3(const vec2<T>& v, T _z = 0)
      : x(v.x), y(v.y), z(_z) {}
    template <typename U> explicit vec3(const vec3<U>& v)
      : x(v.x), y(v.y), z(v.z) {}
    const T *tptr() const { return &x; }
    
    vec2<T> xy() const { return vec2<T>(x, y); }
    vec2<T> xz() const { return vec2<T>(x, z); }
    vec2<T> yz() const { return vec2<T>(y, z); }
    vec3 xzy() const { return vec3(x, z, y); }
    
    T dot(vec3 r) const { return x*r.x + y*r.y + z*r.z; }
    T length_sq() const { return dot(*this); }
    T length() const { return sqrt(length_sq()); }
    T rlength() const { return rsqrt(length_sq()); }
    T safe_rlength() const {
      T len2 = length_sq();
      return (len2 == 0) ? 0 : rsqrt(len2);
    }
    vec3 recip() const { return vec3(::kapusha::recip(x),
                                     ::kapusha::recip(y),
                                     ::kapusha::recip(z)); }
    vec3 normalized() const { return *this * rlength(); }
    vec3 safe_normalized() const { return *this * safe_rlength(); }
    vec3 normalize() { return *this = normalized(); }
    vec3 cross(const vec3& r) const {
      return vec3(y*r.z - z*r.y, z*r.x - x*r.z, x*r.y - y*r.x);
    }
    vec3 clamped(T min, T max) const {
      return vec3(clamp(x, min, max), clamp(y, min, max), clamp(z, min, max));
    }
    vec3 clamped(vec3 min, vec3 max) const {
      return vec3(clamp(x, min.x, max.x),
                  clamp(y, min.y, max.y),
                  clamp(z, min.z, max.z));
    }
    T min() const { return ::kapusha::min(x, ::kapusha::min(y,z)); }
    T max() const { return ::kapusha::max(x, ::kapusha::max(y,z)); }
    vec3 abs() const { return vec3(::kapusha::abs(x),
                                   ::kapusha::abs(y),
                                   ::kapusha::abs(z)); }
    vec3 mod(vec3 d) const { return vec3(::kapusha::mod(x, d.x),
                                         ::kapusha::mod(y, d.y),
                                         ::kapusha::mod(z, d.z)); }
    
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
} // namespace kapusha
