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
#include "rect4.h"
#include "quat.h"

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
  typedef rect4<float> rect4f;
  typedef quat<float> quatf;
} // namespace kapusha
