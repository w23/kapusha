// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru

#pragma once

#include "vec2.h"

namespace math {
  // OpenGL columns order
  template <typename T>
  struct mat2x2 {
    T m[4];
    
    mat2x2() {}
    
    mat2x2(T v)
    {
      m[0] = m[3] = v;
      m[1] = m[2] = 0;
    }
    
    mat2x2(const vec2<T>& row0, const vec2<T>& row1)
    {
      m[0] = row0.x; m[2] = row0.y;
      m[1] = row1.x; m[3] = row1.y;
    }
    
    mat2x2(T A, T B, T C, T D)
    {
      m[0] = A; m[2] = B;
      m[1] = C; m[3] = D;
    }
    
    T det() const
    {
      return m[0] * m[3] - m[1] * m[2];
    }
    
    mat2x2 operator*(const mat2x2& r) const
    {
      return mat2x2(m[0]*r.m[0] + m[2]*r.m[1], m[0]*r.m[2] + m[2]*r.m[3],
                    m[1]*r.m[0] + m[3]*r.m[1], m[1]*r.m[2] + m[3]*r.m[3]);
    }
    
    static mat2x2 rotation(float angle)
    {
      float s = sinf(angle), c = cosf(angle);
      return mat2x2(c, -s, s, c);
    }
  }; // struct mat2x2
} // namespace math