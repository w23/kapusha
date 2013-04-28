// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include <kapusha/math/math.h>
#include "vec2.h"

namespace kapusha {
  template <typename T> struct vec3 {
    T x, y, z;

    inline vec3() {}
    inline vec3(T v) : x(v), y(v), z(v) {}
    inline vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    inline explicit vec3(const vec2<T>& v, T _z = 0)
      : x(v.x), y(v.y), z(_z) {}
    template <typename U> explicit vec3(const vec3<U>& v)
      : x(v.x), y(v.y), z(v.z) {}
    const T *tptr() const { return &x; }
    
    vec2<T> xy() const { return vec2<T>(x, y); } 
    vec3 xzy() const { return vec3(x, z, y); }
    
    T dot(vec3 r) const { return x*r.x + y*r.y + z*r.z; }
    T length_sq() const { return dot(*this); }
    T length() const { return sqrt(length_sq()); }
    T rlength() const { return rsqrt(length_sq()); }
    vec3 recip() const { return vec3(recip(x), recip(y), recip(z)); }
    vec3 normalized() const { return *this * rlength(); }
    vec3 normalize() { return *this = normalized(); }
    vec3 cross(const vec3& r) const {
      return vec3(y*r.z - z*r.y, z*r.x - x*r.z, x*r.y - y*r.x);
    }
    
    vec3 operator-() const { return vec3(-x, -y, -z); }
    vec3 operator+(const vec3& r) const { return vec3(x+r.x, y+r.y, z+r.z); }
    vec3 operator-(const vec3& r) const { return vec3(x-r.x, y-r.y, z-r.z); }
    vec3 operator*(const vec3& r) const { return vec3(x*r.x, y*r.y, z*r.z); }
    vec3 operator*(T r) const { return vec3(x*r, y*r, z*r); }
    vec3 operator/(const vec3& r) const { return *this * r.recip(); }
    vec3 operator/(T r) const { return *this * recip(r); }
    vec3& operator+=(const vec3& r) { return *this = *this + r; }
    vec3& operator*=(const vec3& r) { return *this = *this * r; }
    vec3& operator/=(const vec3& r) { return *this = *this / r; }
    bool operator==(const vec3 &r) const {
      return r.x == x && r.y == y && r.z == z;
    }
    bool operator!=(const vec3 &r) const {
      return r.x != x || r.y != y || r.z != z;
    }
  }; // vec3
} // namespace kapusha
