// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "vec4.h"

namespace kapusha {
  template <typename T> struct mat4x4 {
    vec4<T> rows[4];

    inline mat4x4() {}
    inline mat4x4(T v) {
      rows[0] = vec4<T>(v, 0, 0, 0);
      rows[1] = vec4<T>(0, v, 0, 0);
      rows[2] = vec4<T>(0, 0, v, 0);
      rows[3] = vec4<T>(0, 0, 0, 1);
    }
    inline mat4x4(const vec4<T>& r0, const vec4<T>& r1, const vec4<T>& r2,
                  const vec4<T>& r3 = vec4<T>(0,0,0,1)) {
      rows[0] = r0; rows[1] = r1; rows[2] = r2; rows[3] = r3;
    }

    mat4x4 transposed() const {
      return mat4x4(
        vec4<T>(rows[0].x, rows[1].x, rows[2].x, rows[3].x),
        vec4<T>(rows[0].y, rows[1].y, rows[2].y, rows[3].y),
        vec4<T>(rows[0].z, rows[1].z, rows[2].z, rows[3].z),
        vec4<T>(rows[0].w, rows[1].w, rows[2].w, rows[3].w));
    }

    mat4x4 operator*(T r) const {
      return mat4x4(rows[0]*r, rows[1]*r, rows[2]*r, rows[3]*r);
    }
    vec4<T> operator*(const vec4<T>& r) const {
      return vec4<T>(rows[0].dot(r), rows[1].dot(r), rows[2].dot(r), rows[3].dot(r));
    }
    vec3<T> operator*(const vec3<T>& r) const { return (*this * vec4<T>(r)).xyz(); }
    mat4x4 operator*(const mat4x4& r) const {
      mat4x4 rt(r.transposed());
#define RRTR(i,j) rows[i].dot(rt.rows[j])
      return mat4x4(
        vec4<T>(RRTR(0,0), RRTR(0,1), RRTR(0,2), RRTR(0,3)),
        vec4<T>(RRTR(1,0), RRTR(1,1), RRTR(1,2), RRTR(1,3)),
        vec4<T>(RRTR(2,0), RRTR(2,1), RRTR(2,2), RRTR(2,3)),
        vec4<T>(RRTR(3,0), RRTR(3,1), RRTR(3,2), RRTR(3,3)));
#undef RRTR
    }

    const T *tptr() const { return rows[0].tptr(); }

    mat4x4 &makeIdentity() { return *this = mat4x4(1); }
    mat4x4 &makeScale(vec3<T> scale) {
      rows[0] = vec4<T>(scale.x, 0,	  0,	   0);
      rows[1] = vec4<T>(0,	 scale.y, 0,	   0);
      rows[2] = vec4<T>(0,	 0,	  scale.z, 0);
      rows[3] = vec4<T>(0,	 0,	  0,	   1);
      return *this;
    }
    mat4x4 &makeTranslation(vec3<T> translation) {
      rows[0] = vec4<T>(1, 0, 0, translation.x);
      rows[1] = vec4<T>(0, 1, 0, translation.y);
      rows[2] = vec4<T>(0, 0, 1, translation.z);
      rows[3] = vec4<T>(0, 0, 0, 1);
      return *this;
    }
    mat4x4 &makeRotation(vec3<T> axis, T angle);

    static mat4x4 make_translation(vec3<T> translation) {
      mat4x4 m;
      m.rows[0] = vec4<T>(1, 0, 0, translation.x);
      m.rows[1] = vec4<T>(0, 1, 0, translation.y);
      m.rows[2] = vec4<T>(0, 0, 1, translation.z);
      m.rows[3] = vec4<T>(0, 0, 0, 1);
      return m;
    }
  }; // mat4x4
} // namespace kapusha
