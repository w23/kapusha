#pragma once
#include "limits.h"
#include "Context.h"
#include "Sampler.h"

namespace kapusha {
  class Framebuffer : public Shareable {
  public:
    Framebuffer();
    ~Framebuffer();

    void attachColor(Context *ctx, Sampler *sampler, unsigned index);
    void attachDepth(Context *ctx);
    //! \todo void attachDepth(Sampler *sampler);
    //! \todo void attachStencil(Sampler *sampler);

    inline void bind(Context *ctx) const { ctx->bindFramebuffer(this); }
  private:
    struct Renderbuffer {
      unsigned name;
      Renderbuffer() : name(0) {}
      ~Renderbuffer() { if (name) glDeleteRenderbuffers(1, &name); }
      void makeDepth(vec2i size);
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
  typedef shared<Framebuffer> SFramebuffer;
} // namespace kapusha