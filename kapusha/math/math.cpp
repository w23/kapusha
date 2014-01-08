// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#include <cstdlib>
#include "types.h"

namespace kapusha {
namespace math {

const f32 c_pi = 3.14159265358979323844f;
const f32 c_2pi = c_pi * 2.f;
const f32 c_pi2 = c_pi * .5f;
const f32 c_pi4 = c_pi * .25f;
const f32 c_kdeg2rad = c_pi / 180.f;
const f32 c_krad2deg = 180.f / c_pi;

f32 frand() { return f32(rand()) / RAND_MAX; }
f32 frand(f32 min, f32 max) { return min + frand() * (max - min); }

} // namespace math
} // namespace kapusha
