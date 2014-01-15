#pragma once
#include <kapusha/render/Material.h>
#include "tagged_array.h"

namespace kapusha {
namespace ooo {

class Material : public core::Object {
public:
  typedef core::shared<Material> shared;

  /// \todo merge with Mesh::uniform_t
  struct value_t {
    enum {
      None, Float, Vec2, Vec3, Vec4, Mat2, Mat4, Sampler
    } type;
    union {
      f32 fp[16];
    } data;
  };

  struct attrib_t {
    int location;
    value_t value;
  };

  struct uniform_t {
    int location;
    value_t value;
  };

  const tagged_array_of_t<attrib_t> &attibs() const { return attribs_; }
  const tagged_array_of_t<uniform_t> &uniforms() const { return uniforms_; }
  render::Material *material() const { return material_; }

public:
  Material(render::Material *material);

  attrib_t *bind_attribute(const tag_t tag, const char *name);
  uniform_t *bind_uniform(const tag_t tag, const char *name);

private:
  render::Material::shared material_;
  tagged_array_of_t<attrib_t> attribs_;
  tagged_array_of_t<uniform_t> uniforms_;
}; // class Material

} // namespace ooo
} // namespace kapusha
