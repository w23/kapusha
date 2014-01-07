// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "vec2.h"

namespace kapusha {
namespace math {
template <typename T> struct mat2x2 {
  vec2<T> rows[2];

  inline mat2x2() {}
  inline mat2x2(T v) { rows[0] = vec2<T>(v, 0); rows[1] = vec2<T>(0, v); }
  mat2x2(const vec2<T>& row0, const vec2<T>& row1) {
    rows[0] = row0; rows[1] = row1;
  }
  inline const T *tptr() const { return rows[0].tptr(); }

  T det() const { vec2<T> v(rows[0].d * rows[1].yx); return v.x - v.y; }
  mat2x2 transposed() const {
    return mat2x2(vec2<T>(rows[0].x, rows[1].x),
      vec2<T>(rows[0].y, rows[1].y));
  }

  vec2<T> operator*(const vec2<T> &r) const {
    return vec2<T>(rows[0].dot(r), rows[1].dot(r));
  }
  mat2x2 operator*(const mat2x2& r) const {
    mat2x2 m;
    m.rows[0] = *this * r.rows[0]; m.rows[1] = *this * r.rows[1];
    *this = m.transposed();
  }

  mat2x2 &makeRotation(T angle) {
    T s = sin(angle), c = cos(angle);
    rows[0] = vec2<T>(c, -s); rows[1] = vec2<T>(s, c);
    return *this;
  }
}; // struct mat2x2
} // namespace math
} // namespace kapusha
