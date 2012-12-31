#include "../core/Core.h"
#include "OpenGL.h"
#include "Texture.h"
#include "Render.h"

namespace kapusha {

#ifndef GL_BGRA
#define GL_BGRA GL_BGRA_EXT
#endif
  
  Texture::Texture()
  {
    glGenTextures(1, &name_);
  }
  
  Texture::~Texture()
  {
    glDeleteTextures(1, &name_);
  }
  
  void Texture::upload(const Meta& meta, void* pixels)
  {
    meta_ = meta;

    L("Loading texture: %dx%d format = %d",
      meta.size.x, meta.size.y, meta.format);
    
    //! \todo table, not switch?
    unsigned internal, format, type;
    switch (meta.format)
    {
      case Meta::RGBA8888:
        internal = GL_RGBA, format = GL_RGBA, type = GL_UNSIGNED_BYTE;
        break;
      case Meta::BGRA8888:
        internal = GL_RGBA, format = GL_BGRA, type = GL_UNSIGNED_BYTE;
        break;
      case Meta::RGB565:
        internal = GL_RGB, format = GL_RGB, type = GL_UNSIGNED_SHORT_5_6_5;
        break;
      default:
        KP_ASSERT(!"Unsupported texture format");
        return;
    }
    
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internal,
                 meta.size.x, meta.size.y, 0,
                 format, type,
                 pixels);
    GL_ASSERT
    
    //! \todo move this to some other place
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GL_ASSERT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GL_ASSERT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    GL_ASSERT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    GL_ASSERT
  }
  
  void Texture::bind() const
  {
    Render::currentRender()->texture().bind(this).commit();
  }
} // namespace kapusha
