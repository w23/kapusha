// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#include "Buffer.h"
#include "Program.h"
#include "Framebuffer.h"
#include "Context.h"

namespace kapusha {
namespace render {
  // TODO static pthread_key_t s_current_context_key_;
  static Context *g_current_context_ = nullptr;
  void Context::set_current(Context *context) {
    KP_ASSERT(!g_current_context_);
    g_current_context_ = context;
  }
  Context *Context::current() {
    KP_ASSERT(g_current_context_);
    return g_current_context_;
  }
  
  void Context::make_current() {
    Context::set_current(this);
  }
  
  void Context::do_bind_buffer(const Buffer *buffer, int binding) {
    if (buffer) {
      if (binding == Buffer::Binding::Native)
		binding = static_cast<int>(buffer->binding_hint());
      glBindBuffer(binding, buffer->name()); GL_ASSERT
    } else {
      KP_ASSERT(binding != Buffer::Binding::Native);
      glBindBuffer(binding, 0);
    }
  }
  
  void Context::do_use_program(const Program *program) {
    glUseProgram(program->name());
  }
  
  void Context::do_bind_sampler(const Sampler *sampler, int unit) {
    if (unit != -1) { glActiveTexture(GL_TEXTURE0 + unit); GL_ASSERT }
    glBindTexture(GL_TEXTURE_2D, sampler->name()); GL_ASSERT
  }
  
  void Context::do_bind_framebuffer(const Framebuffer *framebuffer) {
    if (framebuffer) {
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->name()); GL_ASSERT
#if !KAPUSHA_GLES
      glDrawBuffers(framebuffer->buffersCount(), framebuffer->buffers());
      GL_ASSERT
#endif
    }
    else { glBindFramebuffer(GL_FRAMEBUFFER, 0); GL_ASSERT
      GL_ASSERT
    }
  }
} // namespace render
} // namespace kapusha
