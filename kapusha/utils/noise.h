#pragma once
#include "../math.h"

namespace kapusha {
  f32 hash(int x);
  f32 hash(vec2i p);
  f32 lerp_noise(vec2f pos);
} // namespace kapusha