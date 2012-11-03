#pragma once

#include "../math/types.h"
#include "Program.h"

namespace kapusha {

//! maximum number of stored uniforms
#define MAX_MATERIAL_UNIFORMS 8
  
//! maximum number of floats to store
#define MAX_MATERIAL_UNIFORM_STORAGE 64
  
  //! Basic material that contains uniform values for specific program
  class Material {
  public:
    Material(Program *program);
    virtual ~Material();
    
    //! Make current material -- sets program and uniforms
    void use() const;
    
    //! Get material program
    Program *getProgram() const { return shader_program_; }
    int getUniformLocation(const char* name) {
      return shader_program_->getUniformLocation(name);
    }
    
    //! Set persistent uniform values
    //! These must correspond to names and types in provided program
    void setUniform(const char *name, float value);
    void setUniform(const char *name, const math::vec2f& value);
    void setUniform(const char *name, const math::vec4f& value);
    void setUniform(const char *name, const math::mat2f& value);
    void setUniform(const char *name, const math::mat4f& value);
    void setUniform(int location, float value);
    void setUniform(int location, const math::vec2f& value);
    void setUniform(int location, const math::vec4f& value);
    void setUniform(int location, const math::mat2f& value);
    void setUniform(int location, const math::mat4f& value);
    
  private:
    Program *shader_program_;
    
    struct Uniform {
      enum Type {
        None,
        Float,
        Int,
        Matrix
      } type;
      int location;
      int components;
      int size;
      
      Uniform() : type(None) {}
    };
    Uniform uniforms_[MAX_MATERIAL_UNIFORMS];
    float storage_[MAX_MATERIAL_UNIFORM_STORAGE];
    
    int storage_occupied_;
    
  private:
    inline void setUniform(const char *name, Uniform::Type type,
                           const float* data, int components, int size);
    void setUniform(int location, Uniform::Type type,
                    const float* data, int components, int size);
  };
  
}