// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "OpenGL.h"

namespace kapusha {
  struct BlendState {
    enum Mode {
      Whatever = 0,
      Disabled = 1,
      Enabled = 2
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
      OneMinusSourceAlpha = GL_ONE_MINUS_SRC_ALPHA
      OneMinusDestColor = GL_ONE_MINUS_DST_COLOR,
      OneMinusDestAlpha = GL_ONE_MINUS_DST_ALPHA,
      AlphaSaturate = GL_ALPHA_SATURATE
    };
    enum Equation {
      Add = GL_FUNC_ADD,
      Subtract = GL_FUNC_SUBTRACT,
      ReverseSubtract = GL_FUNC_REVERSE_SUBTRACT
    };
    inline BlendState(Mode mode = Whatever, Equation equ = Add,
                      Function funcSource = ConstOne,
                      Function funcDest = ConstZero,
               vec4f color = vec4f(0.f))
      : mode_(mode), funcSrcColor_(funcSource), fundSrcAlpha_(funcSource)
      , funcDstColor_(funcDest), funcDstAlpha_(funcDest), color_(color) {}
    inline void setMode(Mode mode) { mode_ = mode; }
    inline void enable() { setMode(Enabled); }
    inline void disable() { setMode(Disabled); }
    inline void setFunction(Function source, Function destination) {
      funcSrcColor_ = funcSrcAlpha_ = source;
      funcDstColor_ = funcDstAlpha_ = destination;
    }
    inline void setFunction(Function sourceColor, Function sourceAlpha,
                            Function destColor, Function destAlpha) {
      funcSrcColor_ = sourceColor; funcSrcAlpha_ = sourceAlpha;
      funcDstColor_ = destColor; funcDstAlpha_ = destAlpha;
    }
    inline void setEquation(Equation equ) { equColor_ = equAlpha_ = equ; }
    inline void setEquation(Equation color, Equation alpha) {
      equColor_ = color; equAlpha_ = alpha;
    }
    void apply() const;
  private:
    Mode mode_;
    Function funcSrcColor_, funcSrcAlpha_;
    Function funcDstColor_, funcDstAlpha_;
    Equation equColor_, equAlpha_;
    vec4f color_;
  };

  struct DepthState {
    enum Mode {
      Whatever = 0,
      Disabled = 1,
      Enabled = 2
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
    inline DepthState(Mode mode = Whatever, bool write = true, Comparison comp = Less)
      : mode_(mode), write_(write), comparison_(comp) {}
    inline void setMode(Mode mode) { mode_ = mode; }
    inline void enable() { setMode(Enabled); }
    inline void disable() { setMode(Disabled); }
    inline void setWriteEnable(bool write) { write_ = write; }
    inline void writeEnable() { setWriteEnable(true); }
    inline void writeDisable() { setWriteEnable(false); }
    inline void setComparison(Comparison comparison) { comparison_ = comparison; }
    void apply() const;
  private:
    Mode mode_;
    bool write_;
    Comparison comparison_;
  };
} // namespace kapusha
