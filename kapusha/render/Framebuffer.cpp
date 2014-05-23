#include "Framebuffer.h"

#if defined(GL_DEPTH_COMPONENT24)
#define KP_DEPTH_COMPONENT GL_DEPTH_COMPONENT24
#else
#define KP_DEPTH_COMPONENT GL_DEPTH_COMPONENT16
#endif

namespace kapusha {
namespace render {

Framebuffer::Framebuffer() : n_bufs_(0) { glGenFramebuffers(1, &name_); }
Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &name_); }

void Framebuffer::attach_color(Sampler *sampler, unsigned index) {
  KP_ASSERT(index < MAX_FRAMEBUFFER_ATTACHMENTS);
  color_attachments_[index].reset(sampler);
  bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, sampler->name(), 0);
  GL_ASSERT
  
  switch (glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      KP_ASSERT(!"glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
  case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
      KP_ASSERT(!"glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
#endif //ifdef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
    KP_ASSERT(!"glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
  case GL_FRAMEBUFFER_UNSUPPORTED:
    KP_ASSERT(!"glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_UNSUPPORTED");
  }
  
  n_bufs_ = 0;
  for (int i = 0; i < MAX_FRAMEBUFFER_ATTACHMENTS; ++i)
    if (color_attachments_[i].get()) {
      buffers_[n_bufs_] = GL_COLOR_ATTACHMENT0 + i;
      ++n_bufs_;
    }
}

void Framebuffer::Renderbuffer::make_depth(vec2i size) {
  if (name_ == 0) glGenRenderbuffers(1, &name_);
  GL_ASSERT
  glBindRenderbuffer(GL_RENDERBUFFER, name_);
  GL_ASSERT
  glRenderbufferStorage(GL_RENDERBUFFER, KP_DEPTH_COMPONENT, size.x, size.y);
}

void Framebuffer::attach_depth() {
  bind();
  KP_ASSERT(color_attachments_[0]);
  depth_attachment_rb_.make_depth(color_attachments_[0]->meta().size);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER,
                            depth_attachment_rb_.name());
  GL_ASSERT
  KP_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

} // namespace render
} // namespace kapusha
