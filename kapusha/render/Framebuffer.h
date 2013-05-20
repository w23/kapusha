#pragma once
#include "limits.h"
#include "Sampler.h"

namespace kapusha {
  class Context;
  class Framebuffer : public Shareable {
  public:
    Framebuffer();
    ~Framebuffer();

    void attachColor(Context *ctx, Sampler *sampler, unsigned index);
    //! \todo void attachDepth(Sampler *sampler);
    //! \todo void attachStencil(Sampler *sampler);
    
    void isComplete();
    void bind(Context *ctx);
  protected:
    friend class Context;
    unsigned name() const { return name_; }
  private:
    unsigned name_;
    SSampler colorAttachments_[MAX_FRAMEBUFFER_ATTACHMENTS];
    //SSampler depthAttachment_;
    //SSampler stencilAttachment_;
    bool complete_;
  };
  typedef shared<Framebuffer> SFramebuffer;
} // namespace kapusha