// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#include "Buffer.h"
#include "Material.h"
#include "Framebuffer.h"
#include "Context.h"

namespace kapusha {
namespace render {

void Context::blendstate_t::update(const blend_t &other) {
  if (other.mode != mode) {
    mode = other.mode;
    if (mode == Disabled) glDisable(GL_BLEND); else glEnable(GL_BLEND);
  }
  if (mode == Enabled) {
    if (other.color != color) {
      color = other.color;
      glBlendColor(color.x, color.y, color.z, color.w);
    }

    if (other.equ_color != equ_color || other.equ_alpha != equ_alpha) {
      equ_color = other.equ_color;
      equ_alpha = other.equ_alpha;
      glBlendEquationSeparate(equ_color, equ_alpha);
    }

    if (other.func_src_color != func_src_color ||
      other.func_src_alpha != func_src_alpha ||
      other.func_dst_color != func_dst_color ||
      other.func_dst_alpha != func_dst_alpha) {

      glBlendFuncSeparate(func_src_color, func_dst_color,
        func_src_alpha, func_dst_alpha);
    }
  }
}

void Context::depthstate_t::update(const depth_t &other) {
  if (other.mode != mode) {
    mode = other.mode;
    if (mode == Disabled) glDisable(GL_DEPTH_TEST); else glEnable(GL_DEPTH_TEST);
  }

  if (mode == Enabled) {
    if (other.write != write) {
      write = other.write;
      glDepthMask(write);
    }

    if (other.comparison != comparison) {
      comparison = other.comparison;
      glDepthFunc(comparison);
    }
  }
}

void Context::cullstate_t::update(const cull_t &other) {
  if (other.mode != mode) {
    mode = other.mode;
    if (mode == Disabled) glDisable(GL_CULL_FACE); else glEnable(GL_CULL_FACE);
  }

  if (mode == Enabled) {
    if (other.cull_face != cull_face) {
      cull_face = other.cull_face;
      glCullFace(cull_face);
    }

    if (other.front_face != front_face) {
      front_face = other.front_face;
      glFrontFace(front_face);
    }
  }
}

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
    glDrawBuffers(framebuffer->buffers_count(), framebuffer->buffers());
    GL_ASSERT
#endif
  }
  else { glBindFramebuffer(GL_FRAMEBUFFER, 0); GL_ASSERT
    GL_ASSERT
  }
}

void Context::do_use_material(const Material *material) {
  blend_.update(material->blend());
  depth_.update(material->depth());
  cull_.update(material->cull());
  use_program(material->program());
  material->uniform_state().apply();
}

} // namespace render
} // namespace kapusha
