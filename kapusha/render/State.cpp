// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "OpenGL.h"

namespace kapusha {
  void BlendState::apply() const {
    if (mode_ == Whatever) return;
    if (mode_ == Disabled) { glDisable(GL_BLEND); return; }
    glEnable(GL_BLEND);
    glBlendColor(color_.x, color_.y, color_.z, color_.w);
    glBlendEquationSeparate(equColor_, equAlpha_);
    glBlendFuncSeparate(funcSrcColor_, funcDstColor_,
                        funcSrdAlpha_, funcDstAlpha_);
  }
  void DepthState::apply() const {
    if (mode_ == Whatever) return;
    if (mode_ == Disabled) { glDisable(GL_DEPTH_TEST); return; }
    glEnable(GL_DEPTH_TEST);
    glDepthMask(write_);
    glDepthFunc(comparison_);
  }
} // namespace kapusha
