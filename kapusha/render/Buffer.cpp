// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#include "OpenGL.h"
#include "../io/Stream.h"
#include "Buffer.h"

namespace kapusha {
  Buffer::Buffer(Binding binding_hint) : bindingHint_(binding_hint), size_(0) {
    KP_ASSERT(bindingHint_ != BindingNative);
    glGenBuffers(1, &name_); GL_ASSERT
  }
  Buffer::~Buffer() { glDeleteBuffers(1, &name_); GL_ASSERT }
  void Buffer::load(Context *ctx, void *data, std::size_t size, Usage usage) {
    bind(ctx);
    if (size == size_) {
      glBufferSubData(bindingHint_, 0, size, data); GL_ASSERT
    } else {
      glBufferData(bindingHint_, size, data, usage); GL_ASSERT
      size_ = size;
    }
  }
} // namespace kapusha
