#include "Framebuffer.h"

namespace kapusha {
  Framebuffer::Framebuffer() : n_bufs_(0) { glGenFramebuffers(1, &name_); }
  Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &name_); }
  void Framebuffer::attachColor(Context *ctx, Sampler *sampler, unsigned index) {
    KP_ASSERT(index < MAX_FRAMEBUFFER_ATTACHMENTS);
    colorAttachments_[index].reset(sampler);
    bind(ctx);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, sampler->name(), 0);
    GL_ASSERT
    KP_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    
    n_bufs_ = 0;
    for (int i = 0; i < MAX_FRAMEBUFFER_ATTACHMENTS; ++i)
      if (colorAttachments_[i]) {
        buffers_[n_bufs_] = GL_COLOR_ATTACHMENT0 + i;
        ++n_bufs_;
      }
  }
} // namespace kapusha
