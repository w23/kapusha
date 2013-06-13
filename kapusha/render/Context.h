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
    //! \warning You should destroy only shared contexts you've created yourself
    virtual ~Context() {}
    
    //! Make a new context that shares resources with this one.
    //! Main usage: managing stuff in a separate thread
    //! \warning Must be created before any other GL calls
    virtual Context *createSharedContext() const { return 0; }
    
    //! Make this context current for this thread.
    //! Autocreated context that is passed with IViewportController is already
    //! current
    virtual void makeCurrent() {}
    
    //! \todo bool isMobile();
    //! \todo const char *getVendorString() const;
    //! \todo u32 getVersion() const;
    //! \todo u32 getExtensionsCount() const;
    //! \todo const char *getExtension(u32 index);
    //! \todo bool hasExtension(const char* name);
    //! \todo various limits

    //! Binds a buffer
    void bindBuffer(const Buffer *buffer, int binding);
    
    //! Uses a shader program
    void useProgram(const Program *program);
    
    //! Binds a sampler
    void bindSampler(const Sampler *sampler, int unit);
    
    //! Binds a framebuffer
    void bindFramebuffer(const Framebuffer *framebuffer);
    
  protected:
    Context() {}
  };
} // namespace kapusha
