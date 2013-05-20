// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#include "../core.h"
#include "OpenGL.h"
#include "Context.h"
#include "Sampler.h"
#ifndef GL_BGRA
#define GL_BGRA GL_BGRA_EXT
#endif

namespace kapusha {
  Sampler::Sampler(FilterMode magnification, FilterMode minification) {
    glGenTextures(1, &name_);
    setMagFilter(magnification);
    setMagFilter(minification);
  }
  Sampler::~Sampler() { glDeleteTextures(1, &name_); }
  void Sampler::setMagFilter(FilterMode filter) {
    KP_ASSERT(filter != Linear && filter != Nearest);
    magnification_ = filter;
  }
  void Sampler::upload(Context *ctx, const Meta& meta, const void* pixels) {
    meta_ = meta;
    L("Loading texture: %dx%d format = %d",
      meta.size.x, meta.size.y, meta.format);
    //! \todo table, not switch?
    unsigned internal, format, type;
    switch (meta.format) {
      case Meta::RGBA8888:
        internal = GL_RGBA, format = GL_RGBA, type = GL_UNSIGNED_BYTE;
        break;
      case Meta::BGRA8888:
        internal = GL_RGBA, format = GL_BGRA, type = GL_UNSIGNED_BYTE;
        break;
      case Meta::RGB565:
        internal = GL_RGB, format = GL_RGB, type = GL_UNSIGNED_SHORT_5_6_5;
        break;
      case Meta::R8:
        internal = GL_LUMINANCE, format = GL_LUMINANCE, type = GL_UNSIGNED_BYTE;
        break;
      default: KP_ASSERT(!"Unsupported texture format"); return;
    }
    bind(ctx);
    glTexImage2D(GL_TEXTURE_2D, 0, internal,
                 meta.size.x, meta.size.y, 0,
                 format, type,
                 pixels); GL_ASSERT
    //! \todo move this to some other place?
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnification_); GL_ASSERT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minification_); GL_ASSERT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); GL_ASSERT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); GL_ASSERT
  }
} // namespace kapusha
