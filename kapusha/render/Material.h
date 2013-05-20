// kapusha/render
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "Program.h"
#include "State.h"

namespace kapusha {
  class Sampler;
  //! Basic material that contains uniform values for specific program
  class Material : public Shareable {
  public:
    inline Material(Program *program) : program_(program) {}
    ~Material() {}
    inline Program *getProgram() const { return program_.get(); }
    inline int getUniformLocation(const char* name) const {
      return program_->getUniformLocation(name);
    }
    //! Set persistent uniform values
    void setUniform(const char *name, float value);
    void setUniform(const char *name, const vec2f& value);
    void setUniform(const char *name, const vec3f& value);
    void setUniform(const char *name, const vec4f& value);
    void setUniform(const char *name, const mat2f& value);
    void setUniform(const char *name, const mat4f& value);
    void setUniform(const char *name, Sampler *sampler);
    Program::UniformState &getUniforms() { return uniforms_; }
    const Program::UniformState &getUniforms() const { return uniforms_; }

    //! Make current material -- sets program and uniforms
    void use(Context *ctx) const;
  private:
    SProgram program_;
    Program::UniformState uniforms_;    
    BlendState blend_;
    DepthState depth_;
  }; // class Material
  typedef shared<Material> SMaterial;
}
