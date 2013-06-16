// kapusha/render
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#include "Sampler.h"
#include "Material.h"

namespace kapusha {
  void Material::use(Context *ctx) const {
    program_->use(ctx);
    uniforms_.apply(ctx);
    blend_.apply();
    depth_.apply();
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
  void Material::setUniform(const char *name, Sampler *sampler) {
    uniforms_.setUniform(getUniformLocation(name), sampler);
  }
} // namespace kapusha
