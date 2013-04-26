#pragma once
#include "../core/shared.h"
#include "OpenGL.h"

namespace kapusha {
  class Render;
  class Stream;
  class Buffer : public Shareable {
  public:
    enum Binding {
      BindingArray = GL_ARRAY_BUFFER,
      BindingIndex = GL_ELEMENT_ARRAY_BUFFER
    };
    enum Usage {
      StreamDraw = GL_STREAM_DRAW,
      StreamRead = GL_STREAM_READ,
      StreamCopy = GL_STREAM_COPY,
      StaticDraw = GL_STATIC_DRAW,
      StaticRead = GL_STATIC_READ,
      StaticCopy = GL_STATIC_COPY,
      DynamicDraw = GL_DYNAMIC_DRAW,
      DynamicRead = GL_DYNAMIC_READ,
      DynamicCopy = GL_DYNAMIC_COPY
    };
    enum Access {
      ReadOnly = GL_READ_ONLY,
      WriteOnly = GL_WRITE_ONLY,
      ReadWrite = GL_READ_WRITE
    };
  public:
    Buffer(Binding binding_hint = BindingArray);
    ~Buffer();
    void load(Stream* data, unsigned size, Usage usage = StaticDraw);
    void load(void* data, unsigned size, Usage usage = StaticDraw);
    inline void alloc(unsigned size, Usage usage = StaticDraw) {
      load(static_cast<void*>(0), size, usage);
    }
    void* map(Access access = WriteOnly);
    void unmap();
  protected:
    friend class Render;
    unsigned name() const { return name_; }
  private: // noncopyable
    Buffer& operator=(const Buffer& right) { return *this; }
    Buffer(const Buffer& right) {}
  private:
    void bind() const;
    Binding bindingHint_;
    unsigned name_;
  };
  typedef shared<Buffer> SBuffer;
} // kapusha
