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
    bind();
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
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    GL_ASSERT
  }
  
  void* Buffer::map(unsigned int size)
  {
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
    GL_ASSERT
    
    void *ret = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    GL_ASSERT
    return ret;
  }
  
  void Buffer::unmap()
  {
    glUnmapBuffer(GL_ARRAY_BUFFER);
  }
  
  void Buffer::bind(Binding binding) const
  {
    static unsigned binding_to_gl_tbl[] = {
      GL_ARRAY_BUFFER,
      GL_ELEMENT_ARRAY_BUFFER
    };
    glBindBuffer(binding_to_gl_tbl[binding], name_);
    GL_ASSERT
  }
}