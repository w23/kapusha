#include "Visual.h"

namespace kapusha {
namespace ooo {

Visual::Visual(Mesh *mesh, Material *material) : material_(material) {
  batch_ = new render::Batch();

  for (u32 i = 0; i < material_->attibs().size(); ++i) {
    const auto ma_attr_info = material_->attibs()[i];
    const auto me_attr = mesh->attrib_by_tag(ma_attr_info.tag);
    KP_ASSERT(me_attr);

    /// \todo respect default values
    if (!me_attr) {
      L("Mesh doesn't have attrib values for attrib tag:%08x", ma_attr_info.tag.tag);
      batch_.reset();
      return;
    }

    batch_->set_attrib_source(ma_attr_info.item->location,
      me_attr->buffer, me_attr->components,
      me_attr->offset, me_attr->stride, me_attr->type, me_attr->normalize);
  }

  for (u32 i = 0; i < material_->uniforms().size(); ++i) {
    const auto ma_uniform_info = material_->uniforms()[i];
    set_uniform(ma_uniform_info.item->location, ma_uniform_info.item->value);
  }

  const Mesh::draw_info_t &di = mesh->draw_info();
  batch_->set_geometry(di.primitive_type, di.first, di.count, di.index_type, di.indices_buffer);
  batch_->set_material(material_->material());
}

void Visual::draw(const drawing_context_t &ctx) {
  if (!batch_) return;
  
  /// \todo do we need to reset to material defaults here?
  for (u32 i = 0; i < ctx.uniforms.size(); ++i) {
    const auto uniform_info = ctx.uniforms[i];
    set_uniform(uniform_info.tag, *uniform_info.item);
  }
  batch_->draw();
}

void Visual::set_uniform(tag_t tag, const Material::value_t &value) {
  const Material::uniform_t *uniform = material_->uniforms()[tag];
  if (uniform) set_uniform(uniform->location, value);
}

void Visual::set_uniform(s32 location, const Material::value_t &value) {
  /// \todo this is bad. Merge ooo::Material::value_t with render::Program::UniformSet
  switch(value.type) {
    case Material::value_t::Float:
      batch_->uniform_state().set_uniform(location, value.data.fp[0]);
    break;
    case Material::value_t::Vec2:
      batch_->uniform_state().set_uniform(location, vec2f(value.data.fp));
    break;
    case Material::value_t::Vec3:
      batch_->uniform_state().set_uniform(location, vec3f(value.data.fp));
    break;
    case Material::value_t::Vec4:
      batch_->uniform_state().set_uniform(location, vec4f(value.data.fp));
    break;
    case Material::value_t::Mat2:
      batch_->uniform_state().set_uniform(location, mat2f(value.data.fp));
    break;
    case Material::value_t::Mat4:
      batch_->uniform_state().set_uniform(location, mat4f(value.data.fp));
    break;
    case Material::value_t::None:
    default:
//      KP_ASSERT(!"Invalid/unsupported uniform value type");
//      L("Invalid/unsupported uniform value type");
      return;
  }
}

} // namespace ooo
} // namespace kapusha

