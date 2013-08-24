// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include <math.h>

namespace kapusha {
  extern const float c_pi;
  extern const float c_2pi;
  extern const float c_pi2;
  extern const float c_pi4;
  extern const float c_kdeg2rad;
  extern const float c_krad2deg;
  
  //! float random
  //! \return random float value in [0..1]
  extern float frand();
  
  //! float random in range
  //! \param min minimum value
  //! \param max maximum value
  //! \return random float value in [min..max]
  extern float frand(float min, float max);
  template <typename T> inline T clamp(T value, T min, T max) {
    return (value < min) ? min : ((value > max) ? max : value);
  }
  template <typename T> inline T max(T a, T b) { return (a < b) ? b : a; }
  template <typename T> inline T min(T a, T b) { return (a < b) ? a : b; }
  
  inline float sqrt(float f) { return sqrtf(f); }
  inline float recip(float f) { return 1.f / f; }
  inline float rsqrt(float f) { return recip(sqrt(f)); }
  inline float abs(float f) { return fabs(f); }

  inline int recip(int i) { return 1 / i; /* lol */ }
  inline double recip(double d) { return 1. / d; }
  
  template <typename T, typename V> inline V lerp(V a, V b, T t) {
    return a + (b - a) * t;
  }
} // namespace kapusha

#include "types.h"
