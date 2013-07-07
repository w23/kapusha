#include "CocoaGLContext.h"

namespace kapusha {
  CocoaGLContext::CocoaGLContext(NSOpenGLContext *cocoaContext)
  : cocoaContext_([cocoaContext retain]) {
    KP_ASSERT(cocoaContext != nil);
  }
  
  CocoaGLContext::~CocoaGLContext() {
    [cocoaContext_ release];
  }
  
  Context *CocoaGLContext::createSharedContext() {
    CGLContextObj cglctxobj = (CGLContextObj)[cocoaContext_ CGLContextObj];
    CGLPixelFormatObj cglpixfmt = CGLGetPixelFormat(cglctxobj);
    NSOpenGLPixelFormat *pixfmt = [[[NSOpenGLPixelFormat alloc]initWithCGLPixelFormatObj:cglpixfmt] autorelease];
    NSOpenGLContext *newContext = [[[NSOpenGLContext alloc]
                                    initWithFormat:pixfmt
                                    shareContext:cocoaContext_] autorelease];
    return new CocoaGLContext(newContext);
  }
  
  void CocoaGLContext::makeCurrent() {
    [cocoaContext_ makeCurrentContext];
  }
} // namespace kapusha