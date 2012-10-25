// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru

#pragma once

#include "vec4.h"

namespace math {
  // OpenGL columns order
  template <typename T>
  struct mat4x4 {
    T m[16];
    
    mat4x4() {}
    
    mat4x4(T v)
    {
      m[0] = m[5] = m[10] = m[15] = v;
      m[1] = m[2] = m[3] = m[4] =
      m[6] = m[7] = m[8] = m[9] =
      m[11] = m[12] = m[13] = m[14] = 0;
    }
    
    mat4x4(const vec4<T>& c0, const vec4<T>& c1, const vec4<T>& c2,
           const vec4<T>& c3 = vec4<T>(0,0,0,1))
    {
      m[0]  = c0.x; m[1]  = c1.x; m[2]  = c2.x; m[3]  = c3.x;
      m[4]  = c0.y; m[5]  = c1.y; m[6]  = c2.y; m[7]  = c3.y;
      m[8]  = c0.z; m[9]  = c1.z; m[10] = c2.z; m[11] = c3.z;
      m[12] = c0.w; m[13] = c1.w; m[14] = c2.w; m[15] = c3.w;
    }
    
    mat4x4<T>& cols(const vec4<T>& c0, const vec4<T>& c1, const vec4<T>& c2,
                    const vec4<T>& c3 = vec4<T>(0,0,0,1))
    {
      m[0]  = c0.x; m[1]  = c0.y; m[2]  = c0.z; m[3]  = c0.w;
      m[4]  = c1.x; m[5]  = c1.y; m[6]  = c1.z; m[7]  = c1.w;
      m[8]  = c2.x; m[9]  = c2.y; m[10] = c2.z; m[11] = c2.w;
      m[12] = c3.x; m[13] = c3.y; m[14] = c3.z; m[15] = c3.w;
      return *this;
    }
    
    mat4x4<T>& translation(const vec4<T>& v)
    {
      *this = mat4x4().cols(vec4<T>(1,0,0,0),
                            vec4<T>(0,1,0,0),
                            vec4<T>(0,0,1,0),
                            vec4<T>(-v.x,-v.y,-v.z,1));
      return *this;
    }
    
    // very useful operators
    mat4x4<T> operator*(const mat4x4& a) const
    {
      mat4x4<T> res;
      __restrict T* r = res.m;
      __restrict T const *m0 = m;
      __restrict T const *m1 = a.m;
      // wat is faster?
#define UNROLL 1
#if !UNROLL
      for (int i = 0; i < 16; ++i)
      {
        r[i] = m0[(i&3)]    * m1[(i&12)]
        + m0[(i&3)+4]  * m1[(i&12)+1]
        + m0[(i&3)+8]  * m1[(i&12)+2]
        + m0[(i&3)+12] * m1[(i&12)+3];
      }
#else
      r[0] = m0[0]*m1[0] + m0[4]*m1[1] + m0[8]*m1[2] + m0[12]*m1[3];
      r[1] = m0[1]*m1[0] + m0[5]*m1[1] + m0[9]*m1[2] + m0[13]*m1[3];
      r[2] = m0[2]*m1[0] + m0[6]*m1[1] + m0[10]*m1[2] + m0[14]*m1[3];
      r[3] = m0[3]*m1[0] + m0[7]*m1[1] + m0[11]*m1[2] + m0[15]*m1[3];
      r[4] = m0[0]*m1[4] + m0[4]*m1[5] + m0[8]*m1[6] + m0[12]*m1[7];
      r[5] = m0[1]*m1[4] + m0[5]*m1[5] + m0[9]*m1[6] + m0[13]*m1[7];
      r[6] = m0[2]*m1[4] + m0[6]*m1[5] + m0[10]*m1[6] + m0[14]*m1[7];
      r[7] = m0[3]*m1[4] + m0[7]*m1[5] + m0[11]*m1[6] + m0[15]*m1[7];
      r[8] = m0[0]*m1[8] + m0[4]*m1[9] + m0[8]*m1[10] + m0[12]*m1[11];
      r[9] = m0[1]*m1[8] + m0[5]*m1[9] + m0[9]*m1[10] + m0[13]*m1[11];
      r[10] = m0[2]*m1[8] + m0[6]*m1[9] + m0[10]*m1[10] + m0[14]*m1[11];
      r[11] = m0[3]*m1[8] + m0[7]*m1[9] + m0[11]*m1[10] + m0[15]*m1[11];
      r[12] = m0[0]*m1[12] + m0[4]*m1[13] + m0[8]*m1[14] + m0[12]*m1[15];
      r[13] = m0[1]*m1[12] + m0[5]*m1[13] + m0[9]*m1[14] + m0[13]*m1[15];
      r[14] = m0[2]*m1[12] + m0[6]*m1[13] + m0[10]*m1[14] + m0[14]*m1[15];
      r[15] = m0[3]*m1[12] + m0[7]*m1[13] + m0[11]*m1[14] + m0[15]*m1[15];
#endif
      return res;
    }
  };
} // namespace math