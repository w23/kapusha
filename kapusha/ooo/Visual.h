#pragma once
#include "Material.h"
#include "Mesh.h"
#include "Camera.h"
#include <kapusha/render/Batch.h>

namespace kapusha {
namespace ooo {

class Visual : public core::Object {
public:
  typedef core::shared<Visual> shared;

  Visual(Mesh *mesh, Material *material);

  struct drawing_context_t {
    const camera_t *camera;
    tagged_array_of_t<Material::value_t> uniforms;
  };

  void draw(const drawing_context_t &ctx);

private:
  Material::shared material_;
  render::Batch::shared batch_;

  void set_uniform(tag_t tag, const Material::value_t &value);
  void set_uniform(s32 location, const Material::value_t &value);
}; // class Visual

} // namespace ooo
} // namespace kapusha
