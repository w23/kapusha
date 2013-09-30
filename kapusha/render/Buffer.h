// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "../core.h"
#include "OpenGL.h"
#include "Context.h"

namespace kapusha {
  class Buffer : public Shareable {
  public:
    enum Binding {
      BindingNative = -1,
      BindingArray = GL_ARRAY_BUFFER,
      BindingIndex = GL_ELEMENT_ARRAY_BUFFER
    };
    enum Usage {
#if !KAPUSHA_GLES
      StreamRead = GL_STREAM_READ,
      StreamCopy = GL_STREAM_COPY,
      StaticRead = GL_STATIC_READ,
      StaticCopy = GL_STATIC_COPY,
      DynamicRead = GL_DYNAMIC_READ,
      DynamicCopy = GL_DYNAMIC_COPY,
#endif
      StreamDraw = GL_STREAM_DRAW,
      StaticDraw = GL_STATIC_DRAW,
      DynamicDraw = GL_DYNAMIC_DRAW
    };
  public:
    Buffer(Binding binding_hint = BindingArray);
    ~Buffer();
    void load(void* data, std::size_t size, Usage usage = StaticDraw);
    inline void alloc(std::size_t size, Usage usage = StaticDraw) {
      load(static_cast<void*>(0), size, usage);
    }
    inline void bind(Binding binding = BindingNative) const {
      Context::bind_buffer(this, binding);
    }
  private:
    Binding binding_hint_;
    u32 name_;
    std::size_t size_;
  private: // noncopyable
    Buffer& operator=(const Buffer& right) { return *this; }
    Buffer(const Buffer& right) {}
  protected:
    friend class Context;
    inline unsigned name() const { return name_; }
    inline Binding binding_hint() const { return binding_hint_; }
  };
  typedef shared<Buffer> SBuffer;
} // namespace kapusha
