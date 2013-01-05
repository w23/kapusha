#pragma once

#include "../math/types.h"
#include "Program.h"

namespace kapusha {

//! maximum number of stored uniforms
#define MAX_MATERIAL_UNIFORMS 8
  
//! maximum number of floats to store
#define MAX_MATERIAL_UNIFORM_STORAGE 64

//! maximum number of textures
#define MAX_TEXTURE_UNIFORM_STORAGE 4
  
  class Render;
  class Texture;
  
  //! Basic material that contains uniform values for specific program
  class Material : public Shared {
  public:
    Material(Program *program);
    virtual ~Material();
    
    //! Make current material -- sets program and uniforms
    void use(Render *r) const;
    
    //! Get material program
    Program *getProgram() const { return shader_program_.get(); }
    int getUniformLocation(const char* name) {
      return shader_program_->getUniformLocation(name);
    }
    
    //! Set persistent uniform values
    //! These must correspond to names and types in provided program
    void setUniform(const char *name, float value);
    void setUniform(const char *name, const vec2f& value);
    void setUniform(const char *name, const vec3f& value);
    void setUniform(const char *name, const vec4f& value);
    void setUniform(const char *name, const mat2f& value);
    void setUniform(const char *name, const mat4f& value);
    void setUniform(int location, float value);
    void setUniform(int location, const vec2f& value);
    void setUniform(int location, const vec3f& value);
    void setUniform(int location, const vec4f& value);
    void setUniform(int location, const mat2f& value);
    void setUniform(int location, const mat4f& value);
    void setTexture(const char *name, const Texture *texture);
    
  private:
    SProgram shader_program_;
    
    struct Uniform {
      enum Type {
        None,
        Float,
        Matrix
      } type;
      int location;
      int components;
      int size;
      
      Uniform() : type(None) {}
    };
    Uniform uniforms_[MAX_MATERIAL_UNIFORMS];
    float storage_[MAX_MATERIAL_UNIFORM_STORAGE];

    struct UniformTexture {
      const ::kapusha::Texture* texture;
      int location;
      UniformTexture() : texture(0) {}
      bool empty() const { return texture == 0; }
    };
    UniformTexture uniforms_texture_[MAX_TEXTURE_UNIFORM_STORAGE];
    
    int storage_occupied_;
    
  private:
    inline void setUniform(const char *name, Uniform::Type type,
                           const float* data, int components, int size);
    void setUniform(int location, Uniform::Type type,
                    const float* data, int components, int size);
  };
  
  typedef shared<Material> SMaterial;
  
}
