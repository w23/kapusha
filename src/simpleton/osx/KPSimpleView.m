#import "kapusha/simpleton.h"
#import "KPSimpleView.h"

////////////////////////////////////////////////////////////////////////////////
@interface KPSimpleView () {
  int argc_;
  const char **argv_;
  NSTimer *timer_;
}
- (void) timerDraw;
@end

////////////////////////////////////////////////////////////////////////////////
@implementation KPSimpleView
- (id) initWithFrame:(NSRect)frame argc:(int)argc argv:(const char **)argv {
  static const NSOpenGLPixelFormatAttribute attribs[] = {
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFADepthSize, 32,
    NSOpenGLPFAAccelerated,
    0
  };
  
  NSOpenGLPixelFormat *pixfmt = [[[NSOpenGLPixelFormat alloc]
                                  initWithAttributes:attribs] autorelease];
  
  KP_ASSERT(pixfmt);
  
  self = [super initWithFrame:frame pixelFormat:pixfmt];
  
  if (!self) return nil;
  
  argc_ = argc;
  argv_ = argv;
  
  return self;
}

- (void) dealloc {
  [super dealloc];
}

- (void) prepareOpenGL {
  GLint swapInt = 1;
  [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

  simpleton_init(argc_, argv_);
  
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
  simpleton_size(self.bounds.size.width, self.bounds.size.height);
}

- (void) drawRect:(NSRect)dirtyRect {
  simpleton_draw((KPtime_ms)(kpSysTimeNs() / 1000000));
  [[self openGLContext] flushBuffer];
}

- (void) timerDraw {
  [self setNeedsDisplay:YES];
}
@end
