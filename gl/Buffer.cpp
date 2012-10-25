#include "OpenGL.h"
#include "../io/Stream.h"
#include "Buffer.h"

namespace kapusha {

  Buffer::Buffer()
  {
    glGenBuffers(1, &buffer_name_);
    GL_ASSERT
  }

  Buffer::~Buffer()
  {
    glDeleteBuffers(1, &buffer_name_);
    GL_ASSERT
  }

  void Buffer::load(Stream *data, unsigned size)
  {
    glBindBuffer(GL_ARRAY_BUFFER, buffer_name_);
    GL_ASSERT

    if (data->size() >= size)
    {
      glBufferData(GL_ARRAY_BUFFER, size, data->read(size), GL_STATIC_DRAW);
      GL_ASSERT
    } else {
      glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
      GL_ASSERT
      SP_ASSERT(!"implement this using data->process<>(size);");
    }
  }

  void Buffer::bindAttribute(int index, int components, int offset, int stride) const
  {
    glBindBuffer(GL_ARRAY_BUFFER, buffer_name_);
    GL_ASSERT

    glVertexAttribPointer(index, components, GL_FLOAT, GL_FALSE,
      stride, reinterpret_cast<void*>(offset));
    GL_ASSERT

    glEnableVertexAttribArray(index);
    GL_ASSERT
  }

}