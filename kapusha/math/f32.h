#pragma once
#include <math.h>
#include "number.h"

namespace kapusha {
namespace math {

/// \brief 32-bit floating point number (float, lol)
struct f32 : public number<float> {
 #if _MSC_VER < 1800
  inline f32() {}
#else
  f32() = default;
#endif

  inline f32(float o) : number(o) {}
  inline f32(number o) : number(o.v) {}
  explicit f32(int o) : number(static_cast<float>(o)) {}
  explicit f32(double o) : number(static_cast<float>(o)) {}

  //return f - d * ::floorf(f / d);

  inline f32 abs() const { return ::fabs(v); }
  inline f32 floor() const { return ::floorf(v); }
  inline f32 fract() const { return floor() - *this; }
  inline f32 distance(const f32 &other) const { return other.v - v; }
  inline f32 recip() const { return 1.f / v; }
  inline f32 sqrt() const { return ::sqrtf(v); }
  inline f32 rsqrt() const { return sqrt().recip(); } 
  inline f32 sin() const { return ::sinf(v); }
  inline f32 cos() const { return ::cosf(v); }
  inline f32 tan() const { return ::tanf(v); }

private:
  template <typename U>
  inline explicit f32(U u);
}; // struct f32

//inline f32 operator+(float left, f32 right) { return f32(left) + right; }
} // namespace math
} // namespace kapusha
