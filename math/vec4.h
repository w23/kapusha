// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru

#pragma once

#include <math.h>
#include "vec2.h"
#include "vec3.h"

namespace math {

  template <typename T>
  struct vec4
  {
    T x, y, z, w;
    
    // constructors
    vec4() {}
    
    vec4(T _x, T _y, T _z = 0, T _w = 0)
    : x(_x), y(_y), z(_z), w(_w) {}
    
    vec4(T v) : x(v), y(v), z(v), w(v) {}
    
    vec4(const vec3<T>& v, T _w = 0) : x(v.x), y(v.y), z(v.z), w(_w) {}
    vec4(const vec2<T>& v, T _z = 0, T _w = 0) : x(v.x), y(v.y), z(_z), w(_w) {}
    
    // swizzle
    vec2<T> xy() const
    {
      return vec2<T>(x, y);
    }
    
    vec2<T> zw() const
    {
      return vec2<T>(z, w);
    }

    vec3<T> xyz() const
    {
      return vec3<T>(x, y, z);
    }
    
    // math ops
    T length_sq()
    {
      return x*x + y*y + z*z + w*w;
    }
    
    T length()
    {
      return sqrt(length_sq());
    }
    
    vec4<T> normalize()
    {
      T l = length();
      x /= l; y /= l; z /= l; w /= l;
      return *this;
    }
    
    // operators
    vec4<T> operator-(const vec4<T>& b) const
    {
      return vec4<T>(x-b.x, y-b.y, z-b.z, w-b.w);
    }
    
    vec4<T> operator-() const
    {
      return vec4<T>(-x, -y, -z, -w);
    }
    
    vec4<T>& operator+=(const vec4<T>& b)
    {
      x += b.x;
      y += b.y;
      z += b.z;
      w += b.w;
      return *this;
    }
    
    vec4<T> operator+(const vec4<T>& b) const
    {
      return vec4<T>(x+b.x, y+b.y, z+b.z, w+b.w);
    }
    
    vec4<T> operator*(const vec4<T>& b) const
    {
      return vec4<T>(x*b.x, y*b.y, z*b.z, w*b.w);
    }
    
    vec4<T> operator*(T b) const
    {
      return vec4<T>(x*b, y*b, z*b, w*b);
    }
    
    // cross product
    vec4<T> operator^(const vec4<T>& b) const
    {
      vec4<T> ret;
      ret.x = y * b.z - z * b.y;
      ret.y = z * b.x - x * b.z;
      ret.z = x * b.y - y * b.x;
      ret.w = 0;
      
      return ret;
    }
    
  }; // vec4
    
} // namespace math