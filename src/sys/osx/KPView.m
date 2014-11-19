#import "kapusha/core.h"
#import "KPView.h"

@interface KPView () {
@private
  CVTimeStamp ts_;
}
- (void)presentWithPTS:(const CVTimeStamp*)ts;
@end

static CVReturn kp__ViewDisplayLink(CVDisplayLinkRef displayLink,
  const CVTimeStamp *inNow, const CVTimeStamp *inOutputTime,
  CVOptionFlags flagsIn, CVOptionFlags *flagsOut, void *displayLinkContext)
{
  [(KPView*)displayLinkContext presentWithPTS:inOutputTime];
  return kCVReturnSuccess;
}

@implementation KPView
- (instancetype)initWithSize:(NSSize)size delegate:(id<KPViewDelegate>)delegate
{
  KP_ASSERT(delegate);
  static const NSOpenGLPixelFormatAttribute attribs[] = {
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFADepthSize, 32,
    NSOpenGLPFAAccelerated,
    0
  };
  NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
  KP_ASSERT(pixelFormat);

  self = [super initWithFrame:CGRectMake(0, 0, size.width, size.height) pixelFormat:pixelFormat];
  KP_ASSERT(self);
  
  [self  setWantsBestResolutionOpenGLSurface:YES];

  delegate_ = delegate;

  CVDisplayLinkCreateWithActiveCGDisplays(&display_link_);
  CVDisplayLinkSetOutputCallback(display_link_, &kp__ViewDisplayLink, self);

  return self;
}

- (void) dealloc {
  CVDisplayLinkStop(display_link_);
  CVDisplayLinkRelease(display_link_);

  [super dealloc];
}

- (void) prepareOpenGL {
  [super prepareOpenGL];

  GLint swapInt = 1;
  [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
  
  [delegate_ viewWasInitialized];
  
  CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
  CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
  CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(
    display_link_, cglContext, cglPixelFormat);
  CVDisplayLinkGetCurrentTime(display_link_, &ts_);
  CVDisplayLinkStart(display_link_);
}

- (void) reshape {
  [super reshape];

  CGLLockContext([[self openGLContext] CGLContextObj]);
  [delegate_ viewWasResized:[self convertRectToBacking:self.bounds].size];
  CGLUnlockContext([[self openGLContext] CGLContextObj]);
}

- (void)renewGState {
    [[self window] disableScreenUpdatesUntilFlush];
    [super renewGState];
}

- (void) drawRect:(NSRect)dirtyRect {
  KP_UNUSED(dirtyRect);
  if (CVDisplayLinkIsRunning(display_link_))
    [self presentWithPTS:nil];
}

- (void)viewWillMoveToWindow:(NSWindow *)newWindow {
  if (newWindow != nil) return;
  CVDisplayLinkStop(display_link_);
  
  [[self openGLContext] makeCurrentContext];
  CGLLockContext([[self openGLContext] CGLContextObj]);
  [delegate_ viewWillDisappear];
  CGLFlushDrawable([[self openGLContext] CGLContextObj]);
  CGLUnlockContext([[self openGLContext] CGLContextObj]);
}

- (void) presentWithPTS:(const CVTimeStamp*)ts {
  [[self openGLContext] makeCurrentContext];
  CGLLockContext([[self openGLContext] CGLContextObj]);
  if (ts) kpMemcpy(&ts_, ts, sizeof(ts_));
  [delegate_ viewWillPresent:&ts_];
  CGLFlushDrawable([[self openGLContext] CGLContextObj]);
  CGLUnlockContext([[self openGLContext] CGLContextObj]);
}

- (void)pause {
  CVDisplayLinkStop(display_link_);
}

- (void)resume {
  CVDisplayLinkStart(display_link_);
}

@end
