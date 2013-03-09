#include "OpenGL.h"
#include "../io/Stream.h"
#include "Buffer.h"
#include "Render.h"

namespace kapusha {
  Buffer::Buffer(Binding binding_hint) : bindingHint_(binding_hint) {
    glGenBuffers(1, &name_); GL_ASSERT
  }
  Buffer::~Buffer() {
    //! \todo? ensure it is not bound anywhere
    glDeleteBuffers(1, &name_); GL_ASSERT
  }
  void Buffer::load(Stream *data, unsigned size, Usage usage) {
    bind();
    if (data->size() >= size) {
      glBufferData(bindingHint_, size, data->read(size), usage);
      GL_ASSERT
    } else {
      glBufferData(bindingHint_, size, 0, GL_STATIC_DRAW); GL_ASSERT
      KP_ASSERT(!"implement this using data->process<>(size);");
    }
  }
  void Buffer::load(void *data, unsigned size, Usage usage) {
    bind();
    glBufferData(bindingHint_, size, data, usage); GL_ASSERT
  }
  void* Buffer::map(Access access) {
    bind();
    void *ret = glMapBuffer(GL_ARRAY_BUFFER, access); GL_ASSERT
    return ret;
  }
  void Buffer::unmap() { glUnmapBuffer(bindingHint_); }
  void Buffer::bind() const {
    Render *r = Render::currentRender();
    switch (bindingHint_) {
      case BindingArray: r->bufferArray().bind(this).commit(); break;
      case BindingIndex: r->bufferIndex().bind(this).commit(); break;
    }
  }
}
