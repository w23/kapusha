#pragma once
#include "../math.h"

namespace kapusha {
  using namespace math;
  math::f32 hash(int x);
  math::f32 hash(vec2i p);
  math::f32 lerp_noise(vec2f pos);
} // namespace kapusha