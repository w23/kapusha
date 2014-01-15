#pragma once
#include <kapusha/core/types.h>
#include <cmath>

namespace kapusha {
namespace math {

inline f32 abs(f32 v) { return ::fabs(v); }
inline f32 floor(f32 v) { return ::floorf(v); }
inline f32 fract(f32 v) { return v - floor(v); }
inline f32 distance(f32 a, f32 b) { return b - a; }
inline f32 recip(f32 v) { return 1.f / v; }
inline f32 sqrt(f32 v) { return ::sqrtf(v); }
inline f32 rsqrt(f32 v) { return recip(sqrt(v)); } 
inline f32 sin(f32 v) { return ::sinf(v); }
inline f32 cos(f32 v) { return ::cosf(v); }
inline f32 tan(f32 v) { return ::tanf(v); }
inline f32 copysign(f32 value, f32 sign) {
#if defined(_MSC_VER) && _MSC_VER < 1800
  return static_cast<f32>(::_copysign(value, sign));
#else
  return ::copysign(value, sign);
#endif
}

} // namespace math
} // namespace kapusha
