#include "Framebuffer.h"

namespace kapusha {
  Framebuffer::Framebuffer() { glGenFramebuffers(1, &name_); }
  Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &name_); }
  void Framebuffer::attachColor(Context *ctx, Sampler *sampler, unsigned index) {
    KP_ASSERT(index < 4);
    colorAttachments_[index].reset(sampler);
    bind(ctx);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, 0, sampler->name(), 0);
  }
} // namespace kapusha