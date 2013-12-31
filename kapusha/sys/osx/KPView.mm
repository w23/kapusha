#import <mach/mach_time.h>
#import "KPView.h"
#import "KPView_private.h"
#import "CocoaViewportController.h"

using namespace kapusha;
void core::log::sys_write(const char *message) { NSLog(@"%s", message); }

////////////////////////////////////////////////////////////////////////////////
@interface KPView () {
  vec2i size_;
  CocoaViewportController *viewportController_;
  /// \todo THREADING and CVDisplayLinkRef displayLink_;
  NSTimer *timer_;
}
- (void) mouseAlwaysRelative:(BOOL)always;
- (void) timerDraw;
@end

////////////////////////////////////////////////////////////////////////////////
@implementation KPView
- (id) initWithFrame:(NSRect)frame
     viewportFactory:(const kapusha::IViewportFactory*)viewport_factory {
  static const NSOpenGLPixelFormatAttribute attribs[] = {
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFADepthSize, 32,
    NSOpenGLPFAAccelerated,
    //NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core
    0
  };
  
  NSOpenGLPixelFormat *pixfmt = [[[NSOpenGLPixelFormat alloc]
                                  initWithAttributes:attribs] autorelease];
  KP_ASSERT(pixfmt);
  
  if (self = [super initWithFrame:frame pixelFormat:pixfmt])
    viewportController_ = new CocoaViewportController(self, viewport_factory);
  
  return self;
}

- (void) dealloc {
  delete viewportController_;
  [super dealloc];
}

- (void) prepareOpenGL {
  GLint swapInt = 1;
  [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
  viewportController_->init();
  
  timer_ = [NSTimer timerWithTimeInterval:0.001
                                   target:self
                                 selector:@selector(timerDraw)
                                 userInfo:nil
                                  repeats:YES];
  [[NSRunLoop currentRunLoop] addTimer:timer_
                               forMode:NSDefaultRunLoopMode];
  [[NSRunLoop currentRunLoop] addTimer:timer_
                               forMode:NSEventTrackingRunLoopMode];
  [[self window] setAcceptsMouseMovedEvents:YES];
}

- (void) reshape {
  vec2i size = vec2i(self.bounds.size.width, self.bounds.size.height);
  viewportController_->resize(size);
}

- (void) drawRect:(NSRect)dirtyRect {
  viewportController_->draw();
  [[self openGLContext] flushBuffer];
}

- (BOOL)acceptsFirstResponder { return YES; }
- (BOOL)becomeFirstResponder { return YES; }
- (BOOL)resignFirstResponder { return YES; }
- (void)mouseAlwaysRelative:(BOOL)relative {
  KP_ENSURE(kCGErrorSuccess==CGAssociateMouseAndMouseCursorPosition(!relative));
}
#pragma mark Mouse events
//! \todo right, middle buttons, wheel, maybe less functions
- (void) mouseDragged:(NSEvent *)theEvent { [self mouseMoved:theEvent]; }
- (void) mouseMoved:(NSEvent *)theEvent {
  viewportController_->mouse_moved(self, theEvent);
}

- (void) mouseDown:(NSEvent *)theEvent {
  viewportController_->mouse_down(self, theEvent);
}

- (void) mouseUp:(NSEvent *)theEvent {
  viewportController_->mouse_up(self, theEvent);
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

- (void) timerDraw {
  [self setNeedsDisplay:YES];
}

- (void) requestRedraw {
  [self setNeedsDisplay:YES];
}
@end
