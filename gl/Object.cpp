#include "../sys/System.h"
#include "OpenGL.h"
#include "Program.h"
#include "Buffer.h"
#include "Object.h"

namespace kapusha {

  unsigned Object::gl_geometry_type_table[_GeometryTypes] = {
    GL_TRIANGLES,
    GL_TRIANGLE_STRIP,
    GL_TRIANGLE_FAN,
    GL_POINTS,
    GL_LINES,
    GL_LINE_STRIP
  };

  Object::Object()
    : shader_program_(0)
    , indices_(0)
  {
  }

  Object::~Object()
  {
  }

  void Object::setAttribSource(const char* attrib_name,
      Buffer* buffer, int components, int offset, int stride)
  {
    KP_ASSERT(shader_program_);

    int i = 0;
    for (; i < MAX_ATTRIBS; ++i)
      if (attribs_[i].index == -1) break;
    
    KP_ASSERT(i < MAX_ATTRIBS);

    attribs_[i].index = shader_program_->getAttributeLocation(attrib_name);
    attribs_[i].buffer = buffer;
    attribs_[i].components = components;
    attribs_[i].offset = offset;
    attribs_[i].stride = stride;
  }

  void Object::draw() const
  {
    KP_ASSERT(shader_program_);
    shader_program_->use();

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

  void Object::Attrib::bind() const
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