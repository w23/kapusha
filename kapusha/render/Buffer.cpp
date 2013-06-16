// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#include "OpenGL.h"
#include "../io/Stream.h"
#include "Buffer.h"

namespace kapusha {
  Buffer::Buffer(Binding binding_hint) : bindingHint_(binding_hint) {
    glGenBuffers(1, &name_); GL_ASSERT
  }
  Buffer::~Buffer() { glDeleteBuffers(1, &name_); GL_ASSERT }
  void Buffer::load(Context *ctx, void *data, unsigned size, Usage usage) {
    bind(ctx);
    glBufferData(bindingHint_, size, data, usage); GL_ASSERT
  }
} // namespace kapusha
