// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#include "OpenGL.h"
#include "State.h"

namespace kapusha {
namespace render {
void BlendState::apply() const {
  if (mode_ == ModeWhatever) return;
  if (mode_ == ModeDisabled) { glDisable(GL_BLEND); return; }
  GL_ASSERT
  glEnable(GL_BLEND);
  GL_ASSERT
  glBlendColor(color_.x, color_.y, color_.z, color_.w);
  GL_ASSERT
  glBlendEquationSeparate(equColor_, equAlpha_);
  GL_ASSERT
  glBlendFuncSeparate(funcSrcColor_, funcDstColor_,
                      funcSrcAlpha_, funcDstAlpha_);
  GL_ASSERT
}
void DepthState::apply() const {
  if (mode_ == Whatever) return;
  if (mode_ == Disabled) { glDisable(GL_DEPTH_TEST); return; }
  GL_ASSERT
  glEnable(GL_DEPTH_TEST);
  GL_ASSERT
  glDepthMask(write_);
  GL_ASSERT
  glDepthFunc(comparison_);
  GL_ASSERT
}

} // namespace render
} // namespace kapusha
