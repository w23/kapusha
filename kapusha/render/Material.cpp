#include <string.h>
#include "../core/core.h"
#include "Texture.h"
#include "Material.h"
#include "Render.h"

namespace kapusha {
  Material::Material(Program *program) : program_(program) {}
  Material::~Material() {}
  void Material::use(Render *r) const {
    program_->use(r, &uniforms_);
  }
  void Material::setUniform(const char *name, float value) {
    uniforms_.setUniform(getUniformLocation(name), value);
  }
  void Material::setUniform(const char *name, const vec2f& value) {
    uniforms_.setUniform(getUniformLocation(name), value);
  }
  void Material::setUniform(const char *name, const vec3f& value) {
    uniforms_.setUniform(getUniformLocation(name), value);
  }
  void Material::setUniform(const char *name, const vec4f& value) {
    uniforms_.setUniform(getUniformLocation(name), value);
  }
  void Material::setUniform(const char *name, const mat2f& value) {
    uniforms_.setUniform(getUniformLocation(name), value);
  }
  void Material::setUniform(const char *name, const mat4f& value) {
    uniforms_.setUniform(getUniformLocation(name), value);
  }
  void Material::setSampler(const char *name, Texture *sampler) {
    uniforms_.setSampler(getUniformLocation(name), sampler);
  }
} // namespace kapusha
