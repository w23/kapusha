// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "types.h"

#undef min
#undef max

namespace kapusha {
namespace math {

extern const f32 c_pi;
extern const f32 c_2pi;
extern const f32 c_pi2;
extern const f32 c_pi4;
extern const f32 c_kdeg2rad;
extern const f32 c_krad2deg;

/// float random
/// \return random float value in [0..1]
/// \todo this should be in a completely separate module
extern f32 frand();
  
/// float random in range
/// \param min minimum value
/// \param max maximum value
/// \return random float value in [min..max]
extern f32 frand(f32 min, f32 max);
  
template <typename T> inline T clamp(T value, T min, T max) {
  return (value < min) ? min : ((value > max) ? max : value);
}
template <typename T> inline T max(T a, T b) { return (a < b) ? b : a; }  
template <typename T> inline T min(T a, T b) { return (a < b) ? a : b; }

template <typename T, typename V> inline V lerp(V a, V b, T t) {
  return a + (b - a) * t;
}

} // namespace math

using namespace math;
} // namespace kapusha

