#include "Material.h"

namespace kapusha {
namespace ooo {

Material::Material(render::Material *material) : material_(material)
  , attribs_(), uniforms_() {}


Material::attrib_t *Material::bind_attribute(const tag_t tag,
  const char *name) {
  int loc = material_->program()->attrib_location(name);
  KP_ASSERT(loc >= 0);
  if (loc < 0) return nullptr;
  attrib_t &attrib = attribs_.set(tag, nullptr);
  attrib.location = loc;
  return &attrib;
}

Material::uniform_t *Material::bind_uniform(const tag_t tag,
  const char *name) {
  int loc = material_->program()->uniform_location(name);
  KP_ASSERT(loc >= 0);
  if (loc < 0) return nullptr;
  uniform_t &uniform = uniforms_.set(tag, nullptr);
  uniform.location = loc;
  return &uniform;
}

} // namespace ooo
} // namespace kapusha
