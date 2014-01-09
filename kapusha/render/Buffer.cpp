// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#include "OpenGL.h"
#include "Buffer.h"

namespace kapusha {
namespace render {

Buffer::Buffer(Binding binding_hint) : binding_hint_(binding_hint), size_(0) {
  KP_ASSERT(binding_hint_ != Binding::Native);
  glGenBuffers(1, &name_); GL_ASSERT
}
Buffer::~Buffer() { glDeleteBuffers(1, &name_); GL_ASSERT }

void Buffer::load(const void *data, std::size_t size, Usage usage) {
  bind();
  if (size == size_) {
    glBufferSubData(static_cast<GLenum>(binding_hint_), 0, size, data);
    GL_ASSERT
  } else {
    glBufferData(static_cast<GLenum>(binding_hint_), size, data, usage);
    GL_ASSERT
    size_ = size;
  }
}

} // namespace render
} // namespace kapusha
