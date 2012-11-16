// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru

#pragma once

#include <math.h>
#include "vec2.h"

namespace math {

  template <typename T>
  struct vec3
  {
    T x, y, z;
    
    // constructors
    vec3() {}
    
    vec3(T _x, T _y, T _z)
    : x(_x), y(_y), z(_z) {}
    
    vec3(T v) : x(v), y(v), z(v) {}
    
    vec3(const vec2<T>& v, T _z = 0) : x(v.x), y(v.y), z(_z) {}

    template <typename U>
    vec3(const vec3<U>& v) : x(v.x), y(v.y), z(v.z) {}
    
    // swizzle
    vec2<T> xy() const
    {
      return vec2<T>(x, y);
    }

    vec3<T> xzy() const
    {
      return vec3<T>(x, z, y);
    }
    
    // math ops
    T length_sq()
    {
      return x*x + y*y + z*z;
    }
    
    T length()
    {
      return sqrt(length_sq());
    }
    
    vec3<T> normalize()
    {
      T l = length();
      x /= l; y /= l; z /= l;
      return *this;
    }
    
    // operators
    vec3<T> operator-(const vec3<T>& b) const
    {
      return vec3<T>(x-b.x, y-b.y, z-b.z);
    }
    
    vec3<T> operator-() const
    {
      return vec3<T>(-x, -y, -z);
    }
    
    vec3<T>& operator+=(const vec3<T>& b)
    {
      x += b.x;
      y += b.y;
      z += b.z;
      return *this;
    }
    
    vec3<T> operator+(const vec3<T>& b) const
    {
      return vec3<T>(x+b.x, y+b.y, z+b.z);
    }
    
    vec3<T> operator*(const vec3<T>& b) const
    {
      return vec3<T>(x*b.x, y*b.y, z*b.z);
    }
    
    vec3<T> operator*(T b) const
    {
      return vec3<T>(x*b, y*b, z*b);
    }

    vec3<T> operator*=(T r)
    {
      x *= r; y *= r; z*= r;
      return *this;
    }

    vec3<T> operator/=(T v)
    {
      x /= v; y /= v; z /= v;
      return *this;
    }

    // dot product
    T operator&(const vec3<T>& r) const
    {
      return x * r.x + y * r.y + z * r.z;
    }
    
    // cross product
    vec3<T> operator^(const vec3<T>& b) const
    {
      vec3<T> ret;
      ret.x = y * b.z - z * b.y;
      ret.y = z * b.x - x * b.z;
      ret.z = x * b.y - y * b.x;
      
      return ret;
    }
    
  }; // vec3
    
} // namespace math
