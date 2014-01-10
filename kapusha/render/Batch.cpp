#include "../core.h"
#include "OpenGL.h"
#include "Material.h"
#include "Buffer.h"
#include "Batch.h"
#include "Context.h"

namespace kapusha {
namespace render {

Batch::Batch() : count_(0) {}

void Batch::set_attrib_source(int attrib_location,
  Buffer *buffer, u32 components, u32 offset, u32 stride,
  AttribType type, AttribNormalization norm) {
  for (int i = 0; i < MAX_BATCH_ATTRIBS; ++i)
    if (attribs_[i].index == -1) {
      attribs_[i].index = attrib_location;
      attribs_[i].buffer = buffer;
      attribs_[i].type = static_cast<GLenum>(type);
      attribs_[i].normalized = static_cast<GLenum>(norm);
      attribs_[i].components = components;
      attribs_[i].offset = reinterpret_cast<void*>(offset);
      attribs_[i].stride = stride;
      return;
    }
  KP_ASSERT(!"Too many attributes");
}

void Batch::clear_attributes() {
  for (int i = 0; i < MAX_BATCH_ATTRIBS; ++i) {
    attribs_[i].index = -1;
    attribs_[i].buffer.reset();
  }
}

void Batch::draw() const {
  if (count_ == 0) return;
  KP_ASSERT(material_.valid());
  Context::use_material(material_);
  uniform_state_.apply();
  for (int i = 0; i < MAX_BATCH_ATTRIBS; ++i)
    if (attribs_[i].index != -1) attribs_[i].bind();
  if (indices_.valid()) {
    indices_->bind(Buffer::Binding::Index);
    glDrawElements(geometry_type_, count_, index_type_,
      reinterpret_cast<void*>(first_));
    GL_ASSERT
  } else {
    glDrawArrays(geometry_type_, first_, count_);
    GL_ASSERT
  }
  for (int i = 0; i < MAX_BATCH_ATTRIBS; ++i)
    if (attribs_[i].index != -1) attribs_[i].unbind();
}

void Batch::Attrib::bind() const {
  if (buffer.get()) buffer->bind(Buffer::Binding::Array);
  else Context::bind_buffer(nullptr, static_cast<int>(Buffer::Binding::Array));
  glVertexAttribPointer(index, components, type, normalized, stride, offset);
  GL_ASSERT
  glEnableVertexAttribArray(index);
  GL_ASSERT
}

void Batch::Attrib::unbind() const {
  glDisableVertexAttribArray(index);
  GL_ASSERT
}

} // namespace render
} // namespace kapusha
