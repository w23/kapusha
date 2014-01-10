// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "OpenGL.h"
#include "../math.h"

namespace kapusha {
namespace render {

struct blend_t {
  enum Mode {
    Disabled = 0,
    Enabled = 1
  };
  enum Function {
    ConstOne = GL_ONE,
    ConstZero = GL_ZERO,
    ConstColor = GL_CONSTANT_COLOR,
    ConstAlpha = GL_CONSTANT_ALPHA,
    SourceColor = GL_SRC_COLOR,
    SourceAlpha = GL_SRC_ALPHA,
    DestColor = GL_DST_COLOR,
    DestAlpha = GL_DST_ALPHA,
    OneMinusConstColor = GL_ONE_MINUS_CONSTANT_COLOR,
    OneMinusConstAlpha = GL_ONE_MINUS_CONSTANT_COLOR,
    OneMinusSourceColor = GL_ONE_MINUS_SRC_COLOR,
    OneMinusSourceAlpha = GL_ONE_MINUS_SRC_ALPHA,
    OneMinusDestColor = GL_ONE_MINUS_DST_COLOR,
    OneMinusDestAlpha = GL_ONE_MINUS_DST_ALPHA
#ifdef GL_ALPHA_SATURATE
    , AlphaSaturate = GL_ALPHA_SATURATE
#endif // GL_ALPHA_SATURATE
  };
  enum Equation {
    Add = GL_FUNC_ADD,
    Subtract = GL_FUNC_SUBTRACT,
    ReverseSubtract = GL_FUNC_REVERSE_SUBTRACT
  };

  inline blend_t(Mode mode_in = Disabled, Equation equ_in = Add,
    Function func_src_in = ConstOne, Function func_dst_in = ConstZero,
    vec4f color_in = vec4f(0.f))
    : mode(mode_in), func_src_color(func_src_in), func_src_alpha(func_src_in)
    , func_dst_color(func_dst_in), func_dst_alpha(func_dst_in)
    , equ_color(equ_in), equ_alpha(equ_in), color(color_in) {}

  Mode mode;
  Function func_src_color, func_src_alpha;
  Function func_dst_color, func_dst_alpha;
  Equation equ_color, equ_alpha;
  vec4f color;
};

struct depth_t {
  enum Mode {
    Disabled = 0,
    Enabled = 1
  };
  enum Comparison {
    Never = GL_NEVER,
    Always = GL_ALWAYS,
    Less = GL_LESS,
    LessOrEqual = GL_LEQUAL,
    Equal = GL_EQUAL,
    NotEqual = GL_NOTEQUAL,
    GreaterOrEqual = GL_GEQUAL,
    Greater = GL_GREATER
  };

  inline depth_t(Mode mode_in = Disabled, Mode write_in = Enabled,
    Comparison comp_in = LessOrEqual)
    : mode(mode_in), write(write_in), comparison(comp_in) {}

  Mode mode;
  Mode write;
  Comparison comparison;
};

struct cull_t {
  enum Mode { Disabled = 0, Enabled = 1 };
  enum CullFace {
    Front = GL_FRONT,
    Back = GL_BACK,
    FrontAndBack = GL_FRONT_AND_BACK
  };
  enum FrontFace {
    Clockwise = GL_CW,
    Counterclockwise = GL_CCW
  };

  inline cull_t(Mode mode_in = Disabled, CullFace cull_face_in = Back,
    FrontFace front_face_in = Counterclockwise)
    : mode(mode_in), cull_face(cull_face_in), front_face(front_face_in) {}

  Mode mode;
  CullFace cull_face;
  FrontFace front_face;
}; // struct cull_t

} // namespace render
} // namespace kapusha
