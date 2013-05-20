// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "OpenGL.h"

namespace kapusha {
  class Buffer;
  class Program;
  class Sampler;
  class Framebuffer;

  //! A central point where all the ends meat
  class Context {
  public:
    Context() {}
    ~Context() {}
    
    //! \todo const char *getVendorString() const;
    //! \todo u32 getVersion() const;
    //! \todo u32 getExtensionsCount() const;
    //! \todo const char *getExtension(u32 index);

    //! Binds a buffer
    void bindBuffer(const Buffer *buffer, int binding);
    
    //! Uses a shader program
    void useProgram(const Program *program);
    
    //! Binds a sampler
    void bindSampler(const Sampler *sampler, int unit);
    
    //! Binds a framebuffer
    void bindFramebuffer(const Framebuffer *framebuffer);
  private:
  };
} // namespace kapusha