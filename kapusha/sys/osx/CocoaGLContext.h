#pragma once
#import <Cocoa/Cocoa.h>
#include <kapusha/render.h>

namespace kapusha {
  class CocoaGLContext : public Context {
  public:
    CocoaGLContext(NSOpenGLContext *cocoaContext);
    virtual ~CocoaGLContext();
    virtual Context *createSharedContext();
    virtual void makeCurrent();
  private:
    NSOpenGLContext *cocoaContext_;
  };
} // namespace kapusha