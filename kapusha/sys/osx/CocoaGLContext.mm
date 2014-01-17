#include "CocoaGLContext.h"

namespace kapusha {
CocoaGLContext::CocoaGLContext(NSOpenGLContext *cocoaContext)
: cocoa_context_([cocoaContext retain]) {
  KP_ASSERT(cocoaContext != nil);
}

CocoaGLContext::~CocoaGLContext() {
  [cocoa_context_ release];
}

render::Context *CocoaGLContext::create_shared() {
  CGLContextObj cglctxobj = (CGLContextObj)[cocoa_context_ CGLContextObj];
  CGLPixelFormatObj cglpixfmt = CGLGetPixelFormat(cglctxobj);
  NSOpenGLPixelFormat *pixfmt = [[[NSOpenGLPixelFormat alloc]initWithCGLPixelFormatObj:cglpixfmt] autorelease];
  NSOpenGLContext *newContext = [[[NSOpenGLContext alloc]
                                  initWithFormat:pixfmt
                                  shareContext:cocoa_context_] autorelease];
  return new CocoaGLContext(newContext);
}

void CocoaGLContext::make_current() {
  [cocoa_context_ makeCurrentContext];
  Context::make_current();
}
} // namespace kapusha