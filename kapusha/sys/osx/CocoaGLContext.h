#pragma once
#import <Cocoa/Cocoa.h>
#include <kapusha/render.h>

namespace kapusha {
  class CocoaGLContext : public Context {
  public:
    CocoaGLContext(NSOpenGLContext *cocoaContext);
    virtual ~CocoaGLContext();
    virtual Context *create_shared();
    virtual void make_current();
  private:
    NSOpenGLContext *cocoa_context_;
  };
} // namespace kapusha