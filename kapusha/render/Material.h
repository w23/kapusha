#pragma once
#include "../math/types.h"
#include "Program.h"
#include "Limits.h"

namespace kapusha {
  class Render;
  class Texture;
  
  //! Basic material that contains uniform values for specific program
  class Material : public Shareable {
  public:
    Material(Program *program);
    virtual ~Material();
    Program *getProgram() const { return program_.get(); }
    int getUniformLocation(const char* name) {
      return program_->getUniformLocation(name);
    }
    //! Set persistent uniform values
    void setUniform(const char *name, float value);
    void setUniform(const char *name, const vec2f& value);
    void setUniform(const char *name, const vec3f& value);
    void setUniform(const char *name, const vec4f& value);
    void setUniform(const char *name, const mat2f& value);
    void setUniform(const char *name, const mat4f& value);
    void setSampler(const char *name, Texture *sampler);
    Program::UniformState &getUniforms() { return uniforms_; }
    const Program::UniformState &getUniforms() const { return uniforms_; }

    //! Make current material -- sets program and uniforms
    void use(Render *r) const;
  private:
    SProgram program_;
    Program::UniformState uniforms_;    
  }; // class Material
  typedef shared<Material> SMaterial;
}
