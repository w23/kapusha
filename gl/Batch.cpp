#include "../sys/System.h"
#include "OpenGL.h"
#include "Material.h"
#include "Buffer.h"
#include "Batch.h"

namespace kapusha {

  Batch::Batch()
    : material_(0)
    , indices_(0)
  {
  }

  Batch::~Batch()
  {
  }

  void Batch::setAttribSource(const char* attrib_name,
                              Buffer* buffer, unsigned components,
                              unsigned offset, unsigned stride)
  {
    KP_ASSERT(material_);

    int i = 0;
    for (; i < MAX_ATTRIBS; ++i)
      if (attribs_[i].index == -1) break;
    
    KP_ASSERT(i < MAX_ATTRIBS);

    attribs_[i].index = material_->getProgram()->getAttributeLocation(attrib_name);
    attribs_[i].buffer = buffer;
    attribs_[i].components = components;
    attribs_[i].offset = offset;
    attribs_[i].stride = stride;
  }

  void Batch::draw() const
  {
    KP_ASSERT(material_);
    material_->use();

    for (int i = 0; i < MAX_ATTRIBS; ++i)
      if (attribs_[i].index != -1)
        attribs_[i].bind();

    if (indices_)
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_->name());
      GL_ASSERT

      glDrawElements(gl_geometry_type_, count_, GL_UNSIGNED_INT,
        reinterpret_cast<void*>(first_));
      GL_ASSERT
    } else {
      glDrawArrays(gl_geometry_type_, first_, count_);
      GL_ASSERT
    }
  }

  void Batch::Attrib::bind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, buffer->name());
    GL_ASSERT

    glVertexAttribPointer(index, components, GL_FLOAT, GL_FALSE,
      stride, reinterpret_cast<void*>(offset));
    GL_ASSERT

    glEnableVertexAttribArray(index);
    GL_ASSERT
  }

} // namespace kapusha