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
    
    //! Get current thread context
    static Context *current();
    
    //! Make a new context that shares resources with this one.
    //! Main usage: managing stuff in a separate thread
    //! \warning Must be created before any other GL calls
    virtual Context *create_shared() const { return nullptr; }
    
    //! Make this context current for this thread.
    //! Autocreated context that is passed with IViewportController is already
    //! current
    //! \warning platform implementations should also call parent implementation
    virtual void make_current();
    
    //! \todo bool isMobile();
    //! \todo const char *getVendorString() const;
    //! \todo u32 getVersion() const;
    //! \todo u32 getExtensionsCount() const;
    //! \todo const char *getExtension(u32 index);
    //! \todo bool hasExtension(const char* name);
    //! \todo various limits
    
    //! Bind a buffer
    static inline void bind_buffer(const Buffer *buffer, int binding) {
      current()->do_bind_buffer(buffer, binding);
    }
    
    //! Use a shader program
    static inline void use_program(const Program *program) {
      current()->do_use_program(program);
    }
    
    //! Bind a sampler
    static inline void bind_sampler(const Sampler *sampler, int unit) {
      current()->do_bind_sampler(sampler, unit);
    }
    
    //! Bind a framebuffer
    static inline void bind_framebuffer(const Framebuffer *framebuffer) {
      current()->do_bind_framebuffer(framebuffer);
    }

protected: // implementation details
    void do_bind_buffer(const Buffer *buffer, int binding);
    void do_use_program(const Program *program);
    void do_bind_sampler(const Sampler *sampler, int unit);
    void do_bind_framebuffer(const Framebuffer *framebuffer);
    
    inline Context() {}
    
    //! Set current thread context
    //! \warning internals
    static void set_current(Context *context);
    
    // TODO platform-agnostic static pthread_key_t s_current_context_key_;
  };
} // namespace kapusha
