#pragma once
#include "../math.h"

namespace kapusha {
  float hash(int x);
  float hash(vec2i p);
  float lerp_noise(vec2f pos);
} // namespace kapusha