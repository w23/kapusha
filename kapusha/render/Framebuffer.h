#pragma once
#include "limits.h"
#include "Context.h"
#include "Sampler.h"

namespace kapusha {
namespace render {
class Framebuffer : public core::Object {
public:
  typedef core::shared<Framebuffer> shared;

  Framebuffer();
  ~Framebuffer();

  void attach_color(Sampler *sampler, unsigned index);
  void attach_depth();
  /// \todo void attach_depth(Sampler *sampler);

  inline void bind() const { Context::bind_framebuffer(this); }

private:
  struct Renderbuffer {
    Renderbuffer() : name_(0) {}
    ~Renderbuffer() { if (name_) glDeleteRenderbuffers(1, &name_); }
    void make_depth(vec2i size);
    inline GLuint name() const { return name_; }
  private:
    GLuint name_;
  };
  
  unsigned name_;
  Sampler::shared color_attachments_[MAX_FRAMEBUFFER_ATTACHMENTS];
  GLenum buffers_[MAX_FRAMEBUFFER_ATTACHMENTS];
  int n_bufs_;
  Renderbuffer depth_attachment_rb_;
protected:
  friend class Context;
  unsigned name() const { return name_; }
  const GLenum *buffers() const { return buffers_; }
  int buffers_count() const { return n_bufs_; }
}; // class Framebuffer

} // namespace render
} // namespace kapusha