#include <string.h>
#include "../sys/System.h"
#include "Material.h"

namespace kapusha {
  
  Material::Material(Program *program)
    : shader_program_(program)
    , uniforms_count_(0)
    , storage_occupied_(0)
  {
  }
  
  Material::~Material()
  {
  }
  
  void Material::use() const
  {
    shader_program_->use();
    
    for (int i = 0, data = 0; i < uniforms_count_; ++i)
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
        default:
          KP_ASSERT(!"Unsupported uniform type");
      } // switch
    } // for
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
  
  void Material::setUniform(const char *name, Uniform::Type type,
                            const float* data, int components, int size)
  {
    KP_ASSERT((uniforms_count_+1) < MAX_MATERIAL_UNIFORMS);
    KP_ASSERT((storage_occupied_+size) < MAX_MATERIAL_UNIFORM_STORAGE);
    
    Uniform& uni = uniforms_[uniforms_count_];
    uni.type = type;
    uni.location = shader_program_->getUniformLocation(name);
    uni.components = components;
    
    memcpy(storage_ + storage_occupied_, data, size * sizeof(float));
    
    ++uniforms_count_;
    storage_occupied_ += size;
  }
  
} // namespace kapusha