// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "vec4.h"

namespace kapusha {
  // OpenGL columns order
  template <typename T> struct mat4x4 {
    T m[16];
    inline mat4x4() {}
    mat4x4(T v) {
      m[0] = m[5] = m[10] = m[15] = v;
      m[1] = m[2] = m[3] = m[4] =
      m[6] = m[7] = m[8] = m[9] =
      m[11] = m[12] = m[13] = m[14] = 0;
    }
    //! initialize by rows
    inline mat4x4(const vec4<T>& r0, const vec4<T>& r1, const vec4<T>& r2,
                  const vec4<T>& r3 = vec4<T>(0,0,0,1)) {
      m[0]  = r0.x; m[1]  = r1.x; m[2]  = r2.x; m[3]  = r3.x;
      m[4]  = r0.y; m[5]  = r1.y; m[6]  = r2.y; m[7]  = r3.y;
      m[8]  = r0.z; m[9]  = r1.z; m[10] = r2.z; m[11] = r3.z;
      m[12] = r0.w; m[13] = r1.w; m[14] = r2.w; m[15] = r3.w;
    }
    inline mat4x4<T>& cols(const vec4<T>& c0, const vec4<T>& c1,
                           const vec4<T>& c2,
                           const vec4<T>& c3 = vec4<T>(0,0,0,1)) {
      m[0]  = c0.x; m[1]  = c0.y; m[2]  = c0.z; m[3]  = c0.w;
      m[4]  = c1.x; m[5]  = c1.y; m[6]  = c1.z; m[7]  = c1.w;
      m[8]  = c2.x; m[9]  = c2.y; m[10] = c2.z; m[11] = c2.w;
      m[12] = c3.x; m[13] = c3.y; m[14] = c3.z; m[15] = c3.w;
      return *this;
    }
    inline vec4<T>& col(int n) { return *(static_cast<vec4<T>*>(&m[n*4])); }
    inline vec4<T> row(int n) { return vec4<T>(m[n], m[n+4], m[n+8], m[n+12]); }
    inline void setRow(vec4<T> row, int n) {
      m[n] = row.x;
      m[n+4] = row.y;
      m[n+8] = row.z;
      m[n+12] = row.w;
    }
    inline mat4x4<T>& translation(const vec4<T>& v) {
      *this = mat4x4().cols(vec4<T>(1,0,0,0),
                            vec4<T>(0,1,0,0),
                            vec4<T>(0,0,1,0),
                            vec4<T>(v.x,v.y,v.z,1));
      return *this;
    }
    // very useful operators
    mat4x4<T> operator*(T a) const {
      mat4x4<T> res;
      res.m[0] = m[0] * a;
      res.m[1] = m[1] * a;
      res.m[2] = m[2] * a;
      res.m[3] = m[3] * a;
      res.m[4] = m[4] * a;
      res.m[5] = m[5] * a;
      res.m[6] = m[6] * a;
      res.m[7] = m[7] * a;
      res.m[8] = m[8] * a;
      res.m[9] = m[9] * a;
      res.m[10] = m[10] * a;
      res.m[11] = m[11] * a;
      res.m[12] = m[12] * a;
      res.m[13] = m[13] * a;
      res.m[14] = m[14] * a;
      res.m[15] = m[15] * a;
      return res;
    }

    mat4x4<T> operator+(const mat4x4& a) const {
      mat4x4<T> res;
      res.m[0] = m[0] + a.m[0];
      res.m[1] = m[1] + a.m[1];
      res.m[2] = m[2] + a.m[2];
      res.m[3] = m[3] + a.m[3];
      res.m[4] = m[4] + a.m[4];
      res.m[5] = m[5] + a.m[5];
      res.m[6] = m[6] + a.m[6];
      res.m[7] = m[7] + a.m[7];
      res.m[8] = m[8] + a.m[8];
      res.m[9] = m[9] + a.m[9];
      res.m[10] = m[10] + a.m[10];
      res.m[11] = m[11] + a.m[11];
      res.m[12] = m[12] + a.m[12];
      res.m[13] = m[13] + a.m[13];
      res.m[14] = m[14] + a.m[14];
      res.m[15] = m[15] + a.m[15];
      return res;
    }

    mat4x4<T> operator*(const mat4x4& a) const {
      mat4x4<T> res;
      T* r = res.m;
      T const *m0 = m;
      T const *m1 = a.m;
      // wat is faster?
#define UNROLL 1
#if !UNROLL
      for (int i = 0; i < 16; ++i) {
        r[i] = m0[(i&3)]  * m1[(i&12)]
          + m0[(i&3)+4]   * m1[(i&12)+1]
          + m0[(i&3)+8]   * m1[(i&12)+2]
          + m0[(i&3)+12]  * m1[(i&12)+3];
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
    vec4<T> operator*(const vec4<T>& right) const {
      vec4<T> ret;
      ret.x = right.x * m[0] + right.y * m[4] + right.z * m[8] + right.w * m[12];
      ret.y = right.x * m[1] + right.y * m[5] + right.z * m[9] + right.w * m[13];
      ret.z = right.x * m[2] + right.y * m[6] + right.z * m[10] + right.w * m[14];
      ret.w = right.x * m[3] + right.y * m[7] + right.z * m[11] + right.w * m[15];
      return ret;
    }
    vec3<T> operator*(const vec3<T>& right) const {
      vec3<T> ret;
      ret.x = right.x * m[0] + right.y * m[4] + right.z * m[8] + m[12];
      ret.y = right.x * m[1] + right.y * m[5] + right.z * m[9] + m[13];
      ret.z = right.x * m[2] + right.y * m[6] + right.z * m[10] + m[14];
      return ret;
    }
    mat4x4<T>& rotationAroundAxis(vec3<T> axis, T angle);
  };
} // namespace math