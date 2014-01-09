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
namespace render {

  void Sampler::init(FilterMode magnification, FilterMode minification) {
    glGenTextures(1, &name_);
    setMagFilter(magnification);
    setMinFilter(minification);
  }

  Sampler::Sampler(FilterMode magnification, FilterMode minification) {
    init(magnification, minification);
  }

  Sampler::Sampler(const core::Surface *source,
                   FilterMode magnification, FilterMode minification) {
    init(magnification, minification);
    upload(source);
  }

  Sampler::~Sampler() { glDeleteTextures(1, &name_); }

  void Sampler::setMagFilter(FilterMode filter) {
    KP_ASSERT(filter == Linear || filter == Nearest);
    magnification_ = filter;
  }

  void Sampler::upload(const core::Surface::Meta &meta, const void *data) {
    //L("Loading texture: %dx%d format = %d",
    //  meta.size.x, meta.size.y, meta.format);
    //! \todo table, not switch?
    unsigned internal, format, type;
    switch (meta.format) {
      case core::Surface::Meta::RGBA8888:
        internal = GL_RGBA, format = GL_RGBA, type = GL_UNSIGNED_BYTE;
        break;
      case core::Surface::Meta::BGRA8888:
        internal = GL_RGBA, format = GL_BGRA, type = GL_UNSIGNED_BYTE;
        break;
      case core::Surface::Meta::RGB565:
        internal = GL_RGB, format = GL_RGB, type = GL_UNSIGNED_SHORT_5_6_5;
        break;
      case core::Surface::Meta::RG88:
#if KAPUSHA_GLES
        internal = GL_LUMINANCE_ALPHA, format = GL_LUMINANCE_ALPHA, type = GL_UNSIGNED_BYTE;
#else
        internal = GL_RG, format = GL_RG, type = GL_UNSIGNED_BYTE;
#endif
        break;
      case core::Surface::Meta::R8:
        //internal = GL_LUMINANCE, format = GL_LUMINANCE, type = GL_UNSIGNED_BYTE;
        internal = GL_ALPHA, format = GL_ALPHA, type = GL_UNSIGNED_BYTE;
        break;
#if defined(GL_RGBA_FLOAT32_APPLE)
      case core::Surface::Meta::RGBAF32:
        internal = GL_RGBA_FLOAT32_APPLE, format = GL_RGBA, type = GL_FLOAT;
        break;
#endif

      default: KP_ASSERT(!"Unsupported texture format"); return;
    }
    bind();
    if (meta == meta_) {
      if (data) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, meta.size.x, meta.size.y,
          format, type, data); GL_ASSERT }
    } else {
      glTexImage2D(GL_TEXTURE_2D, 0, internal,
                   meta.size.x, meta.size.y, 0,
                   format, type,
                   data); GL_ASSERT
    }
    //! \todo move this to some other place?
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnification_); GL_ASSERT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minification_); GL_ASSERT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); GL_ASSERT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); GL_ASSERT
    meta_ = meta;
  }

} // namespace render
} // namespace kapusha
