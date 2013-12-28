#pragma once
#include "Program.h"
#include "State.h"

namespace kapusha {
  class Sampler;
  //! Basic material that contains uniform values for specific program
  class Material : public Object {
  public:
    inline Material(Program *program) : program_(program) {}
    ~Material() {}
    inline Program *program() const { return program_.get(); }
    inline int get_uniform_location(const char* name) const {
      return program_->get_uniform_location(name);
    }
    //! Set persistent uniform values
    void set_uniform(const char *name, float value);
    void set_uniform(const char *name, const vec2f& value);
    void set_uniform(const char *name, const vec3f& value);
    void set_uniform(const char *name, const vec4f& value);
    void set_uniform(const char *name, const mat2f& value);
    void set_uniform(const char *name, const mat4f& value);
    void set_uniform(const char *name, Sampler *sampler);
    
	Program::UniformState &uniform_state() { return uniform_state_; }
    const Program::UniformState &uniform_state() const {
	  return uniform_state_;
	}
    
	BlendState &blend() { return blend_; }
    //! Make current material -- sets program and uniforms
    void use() const;
  private:
    SProgram program_;
    Program::UniformState uniform_state_;
    BlendState blend_;
    DepthState depth_;
  }; // class Material
  typedef shared<Material> SMaterial;
}
