#include <string.h>
#include "../sys/System.h"
#include "Texture.h"
#include "Material.h"

namespace kapusha {
  
  Material::Material(Program *program)
    : shader_program_(program)
    , storage_occupied_(0)
  {
  }
  
  Material::~Material()
  {
  }
  
  void Material::use() const
  {
    shader_program_->use();
    
    for (int i = 0, data = 0; i < MAX_MATERIAL_UNIFORMS; ++i)
    {
      switch(uniforms_[i].type)
      {
        case Uniform::Float:
          shader_program_->setUniform(uniforms_[i].location, &storage_[data],
                                    uniforms_[i].components);
          data += uniforms_[i].components;
          break;
        case Uniform::Matrix:
          shader_program_->setUniformMatrix(uniforms_[i].location, &storage_[data],
                                            uniforms_[i].components);
          data += uniforms_[i].components * uniforms_[i].components;
          break;
        case Uniform::None:
          i = MAX_MATERIAL_UNIFORMS;
          break;
        default:
          KP_ASSERT(!"Unsupported uniform type");
      } // switch
    } // for

    for (int i = 0; i < MAX_TEXTURE_UNIFORM_STORAGE; ++i)
      if (!uniforms_texture_[i].empty())
      {
        shader_program_->setUniform(uniforms_texture_[i].location, i);
        uniforms_texture_[i].texture->use(i);
      }
  } // use()
  
  void Material::setUniform(const char *name, float value)
  {
    setUniform(name, Uniform::Float, &value, 1, 1);
  }
  
  void Material::setUniform(const char *name, const math::vec2f& value)
  {
    setUniform(name, Uniform::Float, &value.x, 2, 2);
  }
  
  void Material::setUniform(const char *name, const math::vec4f& value)
  {
    setUniform(name, Uniform::Float, &value.x, 4, 4);
  }
  
  void Material::setUniform(const char *name, const math::mat2f& value)
  {
    setUniform(name, Uniform::Matrix, value.m, 2, 4);
  }
  
  void Material::setUniform(const char *name, const math::mat4f& value)
  {
    setUniform(name, Uniform::Matrix, value.m, 4, 16);
  }
  
  void Material::setUniform(int location, float value)
  {
    setUniform(location, Uniform::Float, &value, 1, 1);
  }
  
  void Material::setUniform(int location, const math::vec2f& value)
  {
    setUniform(location, Uniform::Float, &value.x, 2, 2);
  }
  
  void Material::setUniform(int location, const math::vec4f& value)
  {
    setUniform(location, Uniform::Float, &value.x, 4, 4);
  }
  
  void Material::setUniform(int location, const math::mat2f& value)
  {
    setUniform(location, Uniform::Matrix, value.m, 2, 4);
  }
  
  void Material::setUniform(int location, const math::mat4f& value)
  {
    setUniform(location, Uniform::Matrix, value.m, 4, 16);
  }
  
  void Material::setUniform(const char *name, Uniform::Type type,
                            const float* data, int components, int size)
  {
    setUniform(getUniformLocation(name), type, data, components, size);
  }

  void Material::setTexture(const char *name, const Texture *texture)
  {
    for (int i = 0; i < MAX_TEXTURE_UNIFORM_STORAGE; ++i)
      if (uniforms_texture_[i].empty())
      {
        uniforms_texture_[i].location = getUniformLocation(name);
        uniforms_texture_[i].texture = texture;
        return;
      }
      KP_ASSERT(!"Not enough uniform storage for texture");
  }
  
  void Material::setUniform(int location, Uniform::Type type,
                            const float* data, int components, int size)
  {
    int i;
    int offset = 0;
    for (i = 0; i < MAX_MATERIAL_UNIFORMS; ++i)
    {
      Uniform& uni = uniforms_[i];
      if (uni.type == Uniform::None)
      {
        KP_ASSERT((storage_occupied_+size) < MAX_MATERIAL_UNIFORM_STORAGE);
        uni.type = type;
        uni.location = location;
        uni.components = components;
        uni.size = size;
        memcpy(storage_ + storage_occupied_, data, size * sizeof(float));
        storage_occupied_ += size;
        return;
      } else if (uni.location == location)
      {
        KP_ASSERT(type == uni.type);
        KP_ASSERT(components == uni.components);
        KP_ASSERT(size == uni.size);
        memcpy(storage_ + offset, data, size * sizeof(float));
        return;
      }
      offset += uni.size;
    }
    KP_ASSERT(i < MAX_MATERIAL_UNIFORMS);
  }
  
} // namespace kapusha
