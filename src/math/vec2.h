// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru

#pragma once

namespace kapusha {

  template <typename T>
  struct vec2
  {
    T x, y;
    
    typedef T type;
    
    vec2() {}
    
    vec2(T v) : x(v), y(v) {}
    
    vec2(T _x, T _y)
    : x(_x), y(_y)
    {
    }
    
    vec2(const vec2<T>& v)
    : x(v.x), y(v.y) {}
    
    vec2<T>& operator=(const vec2<T>& v)
    {
      x = v.x;
      y = v.y;
      return *this;
    }
    
    template <typename Q>
    vec2<T>& operator=(const vec2<Q>& v)
    {
      x = v.x;
      y = v.y;
      return *this;
    }
    
    vec2& operator+=(const vec2<T>& v)
    {
      x += v.x;
      y += v.y;
      return *this;
    }
    
    vec2 operator+(const vec2<T>& v) const
    {
      return vec2(x+v.x, y+v.y);
    }
    
    vec2& operator-=(const vec2<T>& v)
    {
      x -= v.x;
      y -= v.y;
      return *this;
    }
    
    vec2 operator-(const vec2<T>& v) const
    {
      return vec2(x-v.x, y-v.y);
    }
    
    vec2 operator-() const
    {
      return vec2(-x, -y);
    }
    
    vec2& operator*=(const vec2<T>& v)
    {
      x *= v.x;
      y *= v.y;
      return *this;
    }
    
    vec2& operator/=(T v)
    {
      x /= v;
      y /= v;
      return *this;
    }
    
    vec2& operator*=(T v)
    {
      x *= v;
      y *= v;
      return *this;
    }
    
    template <typename R>
    vec2 operator*(const vec2<R>& v) const
    {
      return vec2(x*v.x, y*v.y);
    }
    
    vec2 operator*(T v) const
    {
      return vec2(x*v, y*v);
    }
    
    template <typename R>
    vec2 operator/(const vec2<R>& v) const
    {
      return vec2(x/v.x, y/v.y);
    }
    
    vec2 operator/(T v) const
    {
      return vec2(x/v, y/v);
    }
    
    T dot(const vec2<T>& v) const
    {
      return x*v.x + y*v.y;
    }
    
    T length_sq() const
    {
      return dot(*this);
    }
    
    T length() const
    {
      return sqrt(length_sq());
    }
    
    vec2<T> normalize()
    {
      T l = length();
      x /= l; y /= l;
      return *this;
    }
    
    bool operator==(const vec2<T>& v) const
    {
      return v.x == x && v.y == y;
    }
    
    bool operator!=(const vec2<T>& v) const
    {
      return v.x != x || v.y != y;
    }
    
    bool operator<(const vec2<T>& v) const
    {
      return length_sq() < v.length_sq();
    }
    
    vec2<T> rotated(T angle)
    {
      float s = sin(angle), c = cos(angle);
      return vec2(x*c-y*s, x*s+y*c);
    }
    
    vec2 abs() const
    {
      return vec2(x<0?-x:x, y<0?-y:y);
    }
  };
    
    template <typename T>
    vec2<T> operator*(T t, const vec2<T>& v)
    {
      return v * t;
    }
  
} // namespace math