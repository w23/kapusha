#pragma once
#include <kapusha/render/Buffer.h>
#include <kapusha/render/Batch.h>
#include "tagged_array.h"

namespace kapusha {
namespace ooo {

class Mesh : public core::Object {
public:
  typedef core::shared<Mesh> shared;

  struct draw_info_t {
    render::Batch::Geometry primitive_type;
    render::Buffer::shared indices_buffer;
    render::Batch::Index index_type;
    u32 first, count;
  };

  /// \todo merge with Material::value_t
  struct uniform_t {
    enum {
      Float, Vec2, Vec3, Vec4, Mat2, Mat4, Sampler
    } type;
    union {
      f32 fp[16];
      render::Sampler *sampler;
    } data;
    ~uniform_t();
  };

  struct attrib_t {
    render::Buffer::shared buffer;
    u32 components;
    render::Batch::AttribType type;
    render::Batch::AttribNormalization normalize;
    u32 offset, stride;
  };

  inline const draw_info_t &draw_info() const {
    return draw_info_;
  }
  inline const attrib_t *attrib_by_tag(tag_t tag) const {
    return attribs_[tag];
  }
  inline const uniform_t *uniform_by_tag(tag_t tag) const {
    return uniforms_[tag];
  }

public:
  Mesh(draw_info_t &draw_info);
  ~Mesh();

  inline void set_attribute(tag_t tag, const attrib_t &attrib) {
    attribs_.set(tag, &attrib);
  }
  inline void set_uniform(tag_t tag, const uniform_t &uniform) {
    uniforms_.set(tag, &uniform);
  }

private:
  draw_info_t draw_info_;
  tagged_array_of_t<attrib_t> attribs_;
  tagged_array_of_t<uniform_t> uniforms_;
}; // class Mesh

} // namespace ooo
} // namespace kapusha
