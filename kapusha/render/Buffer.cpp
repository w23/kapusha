// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#include "OpenGL.h"
#include "../io/Stream.h"
#include "Buffer.h"

namespace kapusha {
  Buffer::Buffer(Binding binding_hint) : binding_hint_(binding_hint), size_(0) {
    KP_ASSERT(binding_hint_ != BindingNative);
    glGenBuffers(1, &name_); GL_ASSERT
  }
  Buffer::~Buffer() { glDeleteBuffers(1, &name_); GL_ASSERT }
  
  void Buffer::load(void *data, std::size_t size, Usage usage) {
    bind();
    if (size == size_) {
      glBufferSubData(binding_hint_, 0, size, data); GL_ASSERT
    } else {
      glBufferData(binding_hint_, size, data, usage); GL_ASSERT
      size_ = size;
    }
  }
} // namespace kapusha
