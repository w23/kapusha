#pragma once
#include "vec2.h"

namespace kapusha {
namespace math {

template <typename T> struct mat2 {
  vec2<T> rows[2];

#if _MSC_VER < 1800
  inline mat2() {}
#else
  inline mat2() = default;
#endif
  inline mat2(T v) { rows[0] = vec2<T>(v, 0); rows[1] = vec2<T>(0, v); }
  inline mat2(const vec2<T>& row0, const vec2<T>& row1) {
    rows[0] = row0; rows[1] = row1;
  }
  inline explicit mat2(const T *p) { rows[0] = vec2<T>(p); rows[1] = vec2<T>(p+2); }
  inline const T *tptr() const { return rows[0].tptr(); }

  vec2<T> operator*(const vec2<T> &r) const {
    return vec2<T>(dot(rows[0], r), dot(rows[1], r));
  }
  mat2 operator*(const mat2& r) const {
    return transpose(mat2(*this * r.rows[0], *this * r.rows[1]));
  }
}; // struct mat2

template <typename T> T trace(mat2<T> m) {
  return m.rows[0].x + m.rows[1].y;
}
template <typename T> T det(mat2<T> m) {
  const vec2<T> v(m.rows[0].xy() * m.rows[1].yx()); return v.x - v.y;
}
template <typename T> mat2<T> transpose(mat2<T> m) {
  return mat2<T>(
    vec2<T>(m.rows[0].x, m.rows[1].x),
    vec2<T>(m.rows[0].y, m.rows[1].y));
}
template <typename T> mat2<T> rotation(T angle) {
  const T s = sin(angle), c = cos(angle);
  return mat2<T>(vec2<T>(c, -s), vec2<T>(s, c));
}

} // namespace math
} // namespace kapusha
