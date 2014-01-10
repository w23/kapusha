#pragma once
#include "Program.h"
#include "State.h"

namespace kapusha {
namespace render {

class Sampler;
//! Basic material that contains uniform values for specific program
class Material : public core::Object {
public:
  typedef core::shared<Material> shared;

  inline Material(Program *program) : program_(program) {}
  ~Material() {}
  
  inline Program *program() const { return program_.get(); }
  inline int uniform_location(const char* name) const {
    return program_->uniform_location(name);
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
  const Program::UniformState &uniform_state() const { return uniform_state_; }

  blend_t &blend() { return blend_; }
  depth_t &depth() { return depth_; }
  cull_t &cull() { return cull_; }
  const blend_t &blend() const { return blend_; }
  const depth_t &depth() const { return depth_; }
  const cull_t &cull() const { return cull_; }

private:
  Program::shared program_;
  Program::UniformState uniform_state_;
  blend_t blend_;
  depth_t depth_;
  cull_t cull_;
}; // class Material

} // namespace render
} // namespace kapusha
