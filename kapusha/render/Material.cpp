// kapusha/render
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#include "Sampler.h"
#include "Material.h"

namespace kapusha {
namespace render {

void Material::use() const {
  program_->use();
  uniform_state_.apply();
  blend_.apply();
  depth_.apply();
}
void Material::set_uniform(const char *name, float value) {
  uniform_state_.set_uniform(uniform_location(name), value);
}
void Material::set_uniform(const char *name, const vec2f& value) {
  uniform_state_.set_uniform(uniform_location(name), value);
}
void Material::set_uniform(const char *name, const vec3f& value) {
  uniform_state_.set_uniform(uniform_location(name), value);
}
void Material::set_uniform(const char *name, const vec4f& value) {
  uniform_state_.set_uniform(uniform_location(name), value);
}
void Material::set_uniform(const char *name, const mat2f& value) {
  uniform_state_.set_uniform(uniform_location(name), value);
}
void Material::set_uniform(const char *name, const mat4f& value) {
  uniform_state_.set_uniform(uniform_location(name), value);
}
void Material::set_uniform(const char *name, Sampler *sampler) {
  uniform_state_.set_uniform(uniform_location(name), sampler);
}

} // namespace render
} // namespace kapusha
