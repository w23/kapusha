// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#include "Buffer.h"
#include "Program.h"
#include "Framebuffer.h"
#include "Context.h"

namespace kapusha {
  void Context::bindBuffer(const Buffer *buffer, int binding) {
    if (binding == Buffer::BindingNative) binding = buffer->bindingHint();
    glBindBuffer(binding, buffer->name()); GL_ASSERT
  }
  void Context::useProgram(const Program *program) {
    glUseProgram(program->name());
  }
  void Context::bindSampler(const Sampler *sampler, int unit) {
    if (unit != -1) { glActiveTexture(GL_TEXTURE0 + unit); GL_ASSERT }
    glBindTexture(GL_TEXTURE_2D, sampler->name()); GL_ASSERT
  }
  void Context::bindFramebuffer(const Framebuffer *framebuffer) {
    if (framebuffer) { glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->name()); GL_ASSERT }
    else { glBindFramebuffer(GL_FRAMEBUFFER, 0); GL_ASSERT }
  }
} // namespace kapusha