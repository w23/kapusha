#include "../core/Log.h"
#include "OpenGL.h"
#include "Texture.h"
#include "Render.h"

namespace kapusha {
  
  unsigned Texture::ImageDesc::getGlFormat() const
  {
    switch (format) {
      case Format_BGRA32:
        return GL_BGRA;
      case Format_RGBA32:
        return GL_RGBA;
      default:
        return GL_INVALID_ENUM;
    }
  }
  
  Texture::Texture()
  : own_(true), name_(0) {}
  
  Texture::Texture(const ImageDesc& desc, unsigned existing_name,
                   bool take_ownership)
  : own_(false), name_(existing_name), desc_(desc)
  {
    if (!existing_name)
      upload(desc, 0);
  }
  
  Texture::~Texture()
  {
    if (own_ && name_)
      glDeleteTextures(1, &name_);
  }
  
  void Texture::upload(const ImageDesc& desc, void* pixels)
  {
    if (!name_ || (!own_ && name_))
      glGenTextures(1, &name_);

    own_ = true;
    
    if (pixels || desc != desc_)
    {
      bind();
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                   desc.size.x, desc.size.y, 0,
                   desc.getGlFormat(), GL_UNSIGNED_BYTE,
                   pixels);
      GL_ASSERT
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      GL_ASSERT
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
      GL_ASSERT
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      GL_ASSERT
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      GL_ASSERT
      
      desc_ = desc;
    }
  }
  
  void Texture::bind() const
  {
    Render::currentRender()->texture().bind(this).commit();
  }
} // namespace kapusha
