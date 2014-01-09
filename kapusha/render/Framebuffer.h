#pragma once
#include "limits.h"
#include "Context.h"
#include "Sampler.h"

namespace kapusha {
namespace render {
  class Framebuffer : public core::Object {
  public:
    Framebuffer();
    ~Framebuffer();

    void attachColor(Sampler *sampler, unsigned index);
    void attachDepth();
    //! \todo void attachDepth(Sampler *sampler);
    //! \todo void attachStencil(Sampler *sampler);

    inline void bind() const { Context::bind_framebuffer(this); }
  private:
    struct Renderbuffer {
      unsigned name;
      Renderbuffer() : name(0) {}
      ~Renderbuffer() { if (name) glDeleteRenderbuffers(1, &name); }
      void makeDepth(math::vec2i size);
    };
    
    unsigned name_;
    SSampler colorAttachments_[MAX_FRAMEBUFFER_ATTACHMENTS];
    GLenum buffers_[MAX_FRAMEBUFFER_ATTACHMENTS];
    int n_bufs_;
    Renderbuffer depthAttachmentRb_;
    //SSampler depthAttachment_;
    //SSampler stencilAttachment_;
    //bool complete_;
  protected:
    friend class Context;
    unsigned name() const { return name_; }
    const GLenum *buffers() const { return buffers_; }
    int buffersCount() const { return n_bufs_; }
  };
  typedef core::shared<Framebuffer> SFramebuffer;

} // namespace render
} // namespace kapusha