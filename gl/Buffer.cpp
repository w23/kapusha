#include "OpenGL.h"
#include "../io/Stream.h"
#include "Buffer.h"

namespace kapusha {

  Buffer::Buffer()
  {
    glGenBuffers(1, &name_);
    GL_ASSERT
  }

  Buffer::~Buffer()
  {
    glDeleteBuffers(1, &name_);
    GL_ASSERT
  }

  void Buffer::load(Stream *data, unsigned size)
  {
    glBindBuffer(GL_ARRAY_BUFFER, name_);
    GL_ASSERT

    if (data->size() >= size)
    {
      glBufferData(GL_ARRAY_BUFFER, size, data->read(size), GL_STATIC_DRAW);
      GL_ASSERT
    } else {
      glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
      GL_ASSERT
      KP_ASSERT(!"implement this using data->process<>(size);");
    }
  }

  void Buffer::load(void *data, unsigned size)
  {
    glBindBuffer(GL_ARRAY_BUFFER, name_);
    GL_ASSERT

    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    GL_ASSERT
  }
}