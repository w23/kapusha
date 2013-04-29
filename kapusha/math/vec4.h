// kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include <kapusha/math.h>

namespace kapusha {
  template <typename T> struct vec4 {
    T x, y, z, w;
    
    inline vec4() {}
    inline vec4(T v) : x(v), y(v), z(v), w(v) {}
    inline vec4(T _x, T _y, T _z = 0, T _w = 0)
      : x(_x), y(_y), z(_z), w(_w) {}
    inline explicit vec4(const vec2<T>& v, T _z = 0, T _w = 0)
      : x(v.x), y(v.y), z(_z), w(_w) {}
    inline explicit vec4(const vec3<T>& v, T _w = 0)
      : x(v.x), y(v.y), z(v.z), w(_w) {}
    
    vec2<T> xy() const { return vec2<T>(x, y); } 
    vec2<T> zw() const { return vec2<T>(z, w); }
    vec3<T> xyz() const { return vec3<T>(x, y, z); }
    
    T dot(vec4 r) const { return x*r.x + y*r.y + z*r.z + w*r.w; }
    T length_sq() const { return dot(*this); }
    T length() const { return sqrt(length_sq()); }
    T rlength() const { return rsqrt(length_sq()); }
    vec4 recip() const { return vec4(recip(x), recip(y), recip(z), recip(w)); }
    vec4 normalized() const { return *this * rlength(); }
    vec4 normalize() { return *this = normalized(); }
    vec4 cross(const vec4<T>& r) const {
      return vec4(y*r.z - z*r.y, z*r.x - x*r.z, x*r.y - y*r.x, 0);
    }
    
    vec4<T> operator-() const { return vec4<T>(-x, -y, -z, -w); }
    vec4<T> operator+(const vec4<T>& r) const {
      return vec4<T>(x+r.x, y+r.y, z+r.z, w+r.w);
    }
    vec4<T> operator-(const vec4<T>& r) const {
      return vec4<T>(x-r.x, y-r.y, z-r.z, w-r.w);
    }
    vec4<T> operator*(const vec4<T>& r) const {
      return vec4<T>(x*r.x, y*r.y, z*r.z, w*r.w);
    }
    vec4<T> operator*(T r) const { return vec4<T>(x*r, y*r, z*r, w*r); }
    vec4<T> operator/(const vec4<T>& r) const { return *this * r.recip(); }
    vec4<T> operator/(T r) const { return *this * recip(r); }
    vec4<T>& operator+=(const vec4<T>& r) { return *this = *this + r; }

    const T *tptr() const { return &x; }
  }; // vec4
} // namespace kapusha
