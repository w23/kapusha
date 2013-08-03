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
    void load(Context *ctx, void* data, std::size_t size, Usage usage = StaticDraw);
    inline void alloc(Context *ctx, std::size_t size, Usage usage = StaticDraw) {
      load(ctx, static_cast<void*>(0), size, usage);
    }
    inline void bind(Context *ctx, Binding binding = BindingNative) const {
      ctx->bindBuffer(this, binding);
    }
  private:
    Binding bindingHint_;
    u32 name_;
    std::size_t size_;
  private: // noncopyable
    Buffer& operator=(const Buffer& right) { return *this; }
    Buffer(const Buffer& right) {}
  protected:
    friend class Context;
    inline unsigned name() const { return name_; }
    inline Binding bindingHint() const { return bindingHint_; }
  };
  typedef shared<Buffer> SBuffer;
} // namespace kapusha
