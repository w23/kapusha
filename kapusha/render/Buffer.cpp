#include "OpenGL.h"
#include "../io/Stream.h"
#include "Buffer.h"

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
  void Buffer::bind(Binding binding) const {
    if (binding == BindingNative) binding = bindingHint_;
    glBindBuffer(binding, name_); GL_ASSERT
  }
}
