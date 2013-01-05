// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru

#pragma once

#include "../core/types.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat2.h"
#include "mat4.h"
#include "rect2.h"

namespace kapusha {
  
  typedef vec2<int> vec2i;
  typedef vec2<float> vec2f;
  
  typedef vec3<float> vec3f;
  typedef vec3<int> vec3i;

  typedef vec4<float> vec4f;
  typedef vec4<u8> vec4b;
  
  typedef mat2x2<float> mat2f;
  typedef mat4x4<float> mat4f;
  
  typedef rect2<int> rect2i;
  typedef rect2<float> rect2f;
  
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
} // namespace math
