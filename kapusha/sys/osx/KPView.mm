#include <mach/mach_time.h>
#import "KPView.h"
#include "CocoaGLContext.h"
#include "CocoaKeyState.h"
#include "CocoaPointerState.h"

using namespace kapusha;
void log::sys_write(const char *message) { NSLog(@"%s", message); }

////////////////////////////////////////////////////////////////////////////////
class CocoaViewportController;
@interface KPView () {
  CocoaViewportController *viewportController_;
  IViewport *viewport_;
  u32 prevFrameTime_;
  BOOL needRedraw_;
  BOOL wasDummyRedraw_;
  CFRunLoopRef runloop_;
  CFRunLoopObserverRef drawObserver_;
  //! \todo CVDisplayLinkRef displayLink_;
  // http://developer.apple.com/library/mac/#qa/qa1385/_index.html
  // why not now? threading! too lazy to think atm
}
- (void) requestRedraw;
- (void) draw;
- (void) startDrawing;
- (void) stopDrawing;
- (void) mouseAlwaysRelative:(BOOL)always;
@end

////////////////////////////////////////////////////////////////////////////////

class CocoaViewportController : public IViewportController
{
public:
  CocoaViewportController(KPView *view, IViewport *viewport)
    : view_(view), viewport_(viewport), glContext_([view openGLContext]) {
      viewport_->init(this, &glContext_);
    }
  ~CocoaViewportController() { viewport_->close(); }
public: // IViewportController
  void quit(int code) {}
  void requestRedraw() { [view_ requestRedraw]; }
  void setRelativeOnlyPointer(bool relative_only) {
    [view_ mouseAlwaysRelative:relative_only];
    pointerState_.setRelative(relative_only);
  }
  void hideCursor(bool hide) {
    if (hide) CGDisplayHideCursor(kCGDirectMainDisplay);
    else CGDisplayShowCursor(kCGDirectMainDisplay);
  }
  const PointerState& pointerState() const { return pointerState_; }
  const KeyState& keyState() const { return keyState_; }
  inline void mouseMoved(NSView *view, NSEvent *event) {
    pointerState_.mouseMoveTo(view, event);
    viewport_->inputPointer(pointerState_);
  }
  inline void mouseDown(NSView *view, NSEvent *event) {
    pointerState_.mouseDown(view, event);
    viewport_->inputPointer(pointerState_);
  }
  inline void mouseUp(NSView *view, NSEvent *event) {
    pointerState_.mouseUp(view, event);
    viewport_->inputPointer(pointerState_);
  }
  inline void key(NSEvent* event, u32 time) {
    if (keyState_.processEvent(event, time))
      viewport_->inputKey(keyState_);
  }
  inline void resize(vec2i size) {
    viewport_->resize(size);
    pointerState_.resize(size);
  }
private:
  KPView *view_;
  IViewport *viewport_;
  CocoaGLContext glContext_;
  CocoaPointerState pointerState_;
  CocoaKeyState keyState_;
};

////////////////////////////////////////////////////////////////////////////////
@implementation KPView
- (id) initWithFrame:(NSRect)frame WithViewport:(kapusha::IViewport*)viewport {
  static const NSOpenGLPixelFormatAttribute attribs[] = {
    NSOpenGLPFADoubleBuffer, 1,
    NSOpenGLPFADepthSize, 24,
    NSOpenGLPFAAccelerated, 1
  };
  NSOpenGLPixelFormat *pixfmt = [[[NSOpenGLPixelFormat alloc]
                                  initWithAttributes:attribs] autorelease];
  if (self = [super initWithFrame:frame pixelFormat:pixfmt]) {
    [self setViewport:viewport];
  }
  return self;
}
- (void) dealloc {
  delete viewportController_;
  [super dealloc];
}
- (void) setViewport:(kapusha::IViewport*)viewport {
  viewport_ = viewport;
  if ([self openGLContext]) {
    [self prepareOpenGL];
    [self reshape];
  }
}
- (void) requestRedraw {
  needRedraw_ = YES;
  CFRunLoopWakeUp(runloop_);
}
// internals
void KPViewDrawObserverCallback(CFRunLoopObserverRef observer,
                                CFRunLoopActivity activity, void *info) {
  KPView *view = (KPView*)info;
  [view draw];
}
- (void) startDrawing {
  if (drawObserver_) return;
  runloop_ = CFRunLoopGetCurrent();
  CFRunLoopObserverContext ctx;
  memset(&ctx, 0, sizeof(ctx));
  ctx.info = self;
  drawObserver_ = CFRunLoopObserverCreate(kCFAllocatorDefault,
                                          kCFRunLoopBeforeWaiting,
                                          YES, 0, KPViewDrawObserverCallback,
                                          &ctx);
  CFRunLoopAddObserver(runloop_, drawObserver_,
                       kCFRunLoopCommonModes);
}
- (void) stopDrawing {
  CFRunLoopRemoveObserver(runloop_, drawObserver_,
                          kCFRunLoopCommonModes);
  CFRelease(drawObserver_);
}
- (void) draw {
  if (viewport_ && needRedraw_) {
    needRedraw_ = NO;
    u32 time = MachTime::now_ms();
    float dt = (wasDummyRedraw_) ? 0.f : (time - prevFrameTime_) / 1000.f;
    viewport_->draw(time, dt);
    [[self openGLContext] flushBuffer];
    prevFrameTime_ = time;
    wasDummyRedraw_ = NO;
  } else
    wasDummyRedraw_ = YES;
}

#pragma mark - delegate methods
- (void) prepareOpenGL {
  MachTime::reset();
  prevFrameTime_ = MachTime::now_ms();
  delete viewportController_;
  viewportController_ = 0;
  if (viewport_)
    viewportController_ = new CocoaViewportController(self, viewport_);
  needRedraw_ = YES;
  wasDummyRedraw_ = YES;
  [self startDrawing];
}

- (void) reshape {
  if (viewport_)
    viewportController_->resize(vec2i(self.bounds.size.width,
                                      self.bounds.size.height));
}

- (void)drawRect:(NSRect)dirtyRect {
  needRedraw_ = YES;
  [self draw];
}

- (BOOL)acceptsFirstResponder { return YES; }
- (BOOL)becomeFirstResponder { return YES; }
- (BOOL)resignFirstResponder { return YES; }
- (void) mouseAlwaysRelative:(BOOL)relative {
  KP_ENSURE(kCGErrorSuccess==CGAssociateMouseAndMouseCursorPosition(!relative));
}
#pragma mark Mouse events
//! \todo right, middle buttons, wheel, maybe less functions
- (void) mouseDragged:(NSEvent *)theEvent { [self mouseMoved:theEvent]; }
- (void) mouseMoved:(NSEvent *)theEvent {
  viewportController_->mouseMoved(self, theEvent);
}
- (void) mouseDown:(NSEvent *)theEvent {
  viewportController_->mouseDown(self, theEvent);
}
- (void) mouseUp:(NSEvent *)theEvent {
  viewportController_->mouseUp(self, theEvent);
}
#pragma mark Key events
- (void) keyDown:(NSEvent *)theEvent {
  if (!theEvent.isARepeat)
    viewportController_->key(theEvent, MachTime::sys_to_ms([theEvent timestamp]));
}
- (void) flagsChanged:(NSEvent *)theEvent {
  viewportController_->key(theEvent, MachTime::sys_to_ms([theEvent timestamp]));
}
- (void) keyUp:(NSEvent *)theEvent {
  viewportController_->key(theEvent, MachTime::sys_to_ms([theEvent timestamp]));
}
@end
