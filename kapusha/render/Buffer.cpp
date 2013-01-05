#include "OpenGL.h"
#include "../io/Stream.h"
#include "Buffer.h"

#include "Render.h"

namespace kapusha {

  Buffer::Buffer(Binding binding_hint)
    : bindingHint_(binding_hint)
  {
    glGenBuffers(1, &name_);
    GL_ASSERT
  }

  Buffer::~Buffer()
  {
    //! \todo? ensure it is not bound anywhere
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
    //! \fixme implement this
    //switch (bindingHint_)
    //{
    //  case BindingArray:
        Render::currentRender()->bufferArray().bind(this).commit();
    //    break;
    //  case BindingIndex:
    //    Render::currentRender()->bufferIndex().bind(this).commit();
    //    break;
    //}
  }
}
