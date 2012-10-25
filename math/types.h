// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru

#pragma once

#include "vec2.h"
#include "vec4.h"
#include "mat2.h"
#include "mat4.h"
#include "rect2.h"

namespace math {
  
  typedef vec2<int> vec2i;
  typedef vec2<float> vec2f;
  
  typedef vec4<float> vec4f;
  
  typedef mat2x2<float> mat2x2f;
  typedef mat4x4<float> mat4x4f;
  
  typedef rect2<int> rect2i;
  typedef rect2<float> rect2f;
  
  extern const float c_pi;
  extern const float c_2pi;
  extern const float c_pi2;
  extern const float c_pi4;
  
} // namespace math
