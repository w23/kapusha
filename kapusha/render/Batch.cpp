// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#include "../core.h"
#include "OpenGL.h"
#include "Material.h"
#include "Buffer.h"
#include "Batch.h"
#include "Context.h"

namespace kapusha {
  void Batch::setAttribSource(int attrib_location,
                              Buffer* buffer, u32 components,
                              u32 offset, u32 stride) {
    for (int i = 0; i < MAX_BATCH_ATTRIBS; ++i)
      if (attribs_[i].index == -1) {
        attribs_[i].index = attrib_location;
        attribs_[i].buffer = buffer;
        attribs_[i].components = components;
        attribs_[i].offset = reinterpret_cast<void*>(offset);
        attribs_[i].stride = stride;
        return;
      }
    KP_ASSERT(!"Too many attributes");
  }
  
  void Batch::clearAttributes() {
    for (int i = 0; i < MAX_BATCH_ATTRIBS; ++i) {
      attribs_[i].index = -1;
      attribs_[i].buffer.reset();
    }
  }
  
  void Batch::draw() const {
    KP_ASSERT(material_.valid());
    material_->use();
    uniforms_.apply();
    for (int i = 0; i < MAX_BATCH_ATTRIBS; ++i)
      if (attribs_[i].index != -1) attribs_[i].bind();
    if (indices_.valid()) {
      indices_->bind(Buffer::BindingIndex);
      glDrawElements(geometryType_, count_, indexType_,
        reinterpret_cast<void*>(first_));
      GL_ASSERT
    } else {
      glDrawArrays(geometryType_, first_, count_);
      GL_ASSERT
    }
    for (int i = 0; i < MAX_BATCH_ATTRIBS; ++i)
      if (attribs_[i].index != -1) attribs_[i].unbind();
  }

  void Batch::Attrib::bind() const {
    if (buffer.get()) buffer->bind(Buffer::BindingArray);
    else Context::bind_buffer(nullptr, Buffer::BindingArray);
    glVertexAttribPointer(index, components, GL_FLOAT, GL_FALSE, stride, offset);
    GL_ASSERT
    glEnableVertexAttribArray(index);
    GL_ASSERT
  }
  void Batch::Attrib::unbind() const {
    glDisableVertexAttribArray(index);
    GL_ASSERT
  }
} // namespace kapusha
