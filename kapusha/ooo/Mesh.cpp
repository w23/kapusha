#include "Mesh.h"

namespace kapusha {
namespace ooo {

#ifndef KP_MESH_RESERVED_ATTRIBS
#define KP_MESH_RESERVED_ATTRIBS 4
#endif

#ifndef KP_MESH_RESERVED_UNIFORMS
#define KP_MESH_RESERVED_UNIFORMS 8
#endif

Mesh::uniform_t::~uniform_t() {
  if (type == Sampler && data.sampler)
    data.sampler->release();
}

Mesh::Mesh(draw_info_t &draw_info) : draw_info_(draw_info)
  , attribs_(KP_MESH_RESERVED_ATTRIBS)
  , uniforms_(KP_MESH_RESERVED_UNIFORMS) {}

Mesh::~Mesh() {}

} // namespace ooo
} // namespace kapusha
