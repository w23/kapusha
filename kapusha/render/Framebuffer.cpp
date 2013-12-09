#include "Framebuffer.h"

#if defined(GL_DEPTH_COMPONENT24)
#define KP_DEPTH_COMPONENT GL_DEPTH_COMPONENT24
#else
#define KP_DEPTH_COMPONENT GL_DEPTH_COMPONENT16
#endif

namespace kapusha {
  Framebuffer::Framebuffer() : n_bufs_(0) { glGenFramebuffers(1, &name_); }
  
  Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &name_); }
  
  void Framebuffer::attachColor(Sampler *sampler, unsigned index) {
    KP_ASSERT(index < MAX_FRAMEBUFFER_ATTACHMENTS);
    colorAttachments_[index].reset(sampler);
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, sampler->name(), 0);
    GL_ASSERT
    KP_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    
    n_bufs_ = 0;
    for (int i = 0; i < MAX_FRAMEBUFFER_ATTACHMENTS; ++i)
      if (colorAttachments_[i].get()) {
        buffers_[n_bufs_] = GL_COLOR_ATTACHMENT0 + i;
        ++n_bufs_;
      }
  }
  
  void Framebuffer::Renderbuffer::makeDepth(vec2i size) {
    if (name == 0) glGenRenderbuffers(1, &name);
    GL_ASSERT
    glBindRenderbuffer(GL_RENDERBUFFER, name);
    GL_ASSERT
    glRenderbufferStorage(GL_RENDERBUFFER, KP_DEPTH_COMPONENT, size.x, size.y);
  }
  
  void Framebuffer::attachDepth() {
    bind();
    KP_ASSERT(colorAttachments_[0].get());
    depthAttachmentRb_.makeDepth(colorAttachments_[0]->meta().size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER,
                              depthAttachmentRb_.name);
    GL_ASSERT
    KP_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
  }
} // namespace kapusha
