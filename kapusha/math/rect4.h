// Kapusha/math
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include <limits>
#include "vec4.h"

namespace kapusha {
namespace math {

template <typename T> struct rect4 {
  vec4<T> min, max;
  rect4() {}
  void clear() {
    min = vec4<T>(std::numeric_limits<T>::max(), 0);
    max = vec4<T>(std::numeric_limits<T>::min(), 0);
  }
  vec4<T> center() const { return (min + max) / 2; }
  void extendToContain(const vec4<T> p) {
    if (p.x < min.x) min.x = p.x;
    if (p.y < min.y) min.y = p.y;
    if (p.z < min.y) min.z = p.z;
    if (p.x > max.x) max.x = p.x;
    if (p.y > max.y) max.y = p.y;
    if (p.z > max.y) max.z = p.z;
  }
}; // struct rect4

} // namespace math
} // namespace kapusha
