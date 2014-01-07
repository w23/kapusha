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
namespace math {

typedef vec2<int> vec2i;
typedef vec2<f32> vec2f;
typedef vec3<f32> vec3f;
typedef vec3<int> vec3i;
typedef vec4<f32> vec4f;
typedef vec4<u8> vec4b;
typedef mat2x2<f32> mat2f;
typedef mat4x4<f32> mat4f;
typedef rect2<int> rect2i;
typedef rect2<f32> rect2f;
typedef rect4<f32> rect4f;
typedef quat<f32> quatf;

} // namespace math
} // namespace kapusha
