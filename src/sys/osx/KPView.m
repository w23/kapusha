#import "KPView.h"
#import "KPWindow.h"

static CVReturn kp__ViewDisplayLink(CVDisplayLinkRef displayLink,
  const CVTimeStamp *inNow, const CVTimeStamp *inOutputTime,
  CVOptionFlags flagsIn, CVOptionFlags *flagsOut, void *displayLinkContext);

////////////////////////////////////////////////////////////////////////////////
@interface KPView () <NSWindowDelegate> {
  KPWindow *native_window_;
  KP__cocoa_window_o window_;
  CVDisplayLinkRef display_link_;
  KPwindow_painter_paint_t paint_;
}
- (void) paintWithPTS:(const CVTimeStamp*)ts;
@end

////////////////////////////////////////////////////////////////////////////////
@implementation KPView
- (id) initWithWindow:(KP__cocoa_window_o)window
{
  static const NSOpenGLPixelFormatAttribute attribs[] = {
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFADepthSize, 32,
    NSOpenGLPFAAccelerated,
    0
  };
  
  NSOpenGLPixelFormat *pixfmt = [[[NSOpenGLPixelFormat alloc]
                                  initWithAttributes:attribs] autorelease];
  KP_ASSERT(pixfmt);
  
  NSRect bounds = NSMakeRect(0, 0, window->width, window->height);

  self = [super initWithFrame:bounds pixelFormat:pixfmt];
  KP_ASSERT(self);
  
  window_ = window;
  paint_.config.header.user_data = window->user_data;
  paint_.config.header.window = window;
  
  CVDisplayLinkCreateWithActiveCGDisplays(&display_link_);
  CVDisplayLinkSetOutputCallback(display_link_, &kp__ViewDisplayLink, self);
  return self;
}

- (void) dealloc {
  [super dealloc];
}

- (void) prepareOpenGL {
  GLint swapInt = 1;
  [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
  
  CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
  CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
  CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(
    display_link_, cglContext, cglPixelFormat);
  
  window_->painter_create_func(&paint_.config.header);
  
  CVDisplayLinkStart(display_link_);
}

- (void) reshape {
  [[self openGLContext] makeCurrentContext];
  CGLLockContext([[self openGLContext] CGLContextObj]);
  [[self openGLContext] update];
  
  paint_.config.width = self.bounds.size.width;
  paint_.config.height = self.bounds.size.height;
  paint_.config.aspect = paint_.config.width * window_->output->parent.hppmm
    / (paint_.config.height * window_->output->parent.vppmm);
  window_->painter_configure_func(&paint_.config);

  CGLUnlockContext([[self openGLContext] CGLContextObj]);
  [self unlockFocus];
}

- (void) drawRect:(NSRect)dirtyRect {
  KP_UNUSED(dirtyRect);
}

- (void) paintWithPTS:(const CVTimeStamp*)ts {
  if (![self lockFocusIfCanDraw]) return;
  
  paint_.pts = ts->videoTime;

  [[self openGLContext] makeCurrentContext];
  CGLLockContext([[self openGLContext] CGLContextObj]);
  window_->painter_func(&paint_);
  [[self openGLContext] flushBuffer];
  CGLUnlockContext([[self openGLContext] CGLContextObj]);
  [self unlockFocus];
}

- (BOOL)windowShouldClose:(id)sender {
  [[NSApplication sharedApplication] terminate:self];
  return YES;
}
@end

static CVReturn kp__ViewDisplayLink(CVDisplayLinkRef displayLink,
  const CVTimeStamp *inNow, const CVTimeStamp *inOutputTime,
  CVOptionFlags flagsIn, CVOptionFlags *flagsOut, void *displayLinkContext)
{
  [(KPView*)displayLinkContext paintWithPTS:inOutputTime];
  return kCVReturnSuccess;
}


KPwindow_o kpWindowCreate(const KPwindow_params_t *params) {
  KP__cocoa_window_o this = KP_NEW(KP__cocoa_window_t, 0);
  KP_ASSERT(params->output);
  this->user_data = params->user_data;
  this->painter_create_func = params->painter_create_func;
  this->painter_configure_func = params->painter_configure_func;
  this->painter_func = params->painter_func;
  this->painter_destroy_func = params->painter_destroy_func;
  this->output = kpRetain(params->output);
  
  CGRect rect;
  rect.origin.x = rect.origin.y = 0;
  if (params->width < 8 || params->height < 8) {
    rect.size.width = this->output->parent.width;
    rect.size.height = this->output->parent.height;
  } else {
    rect.size.width = params->width;
    rect.size.height = params->height;
  }
  
  this->view = [[KPView alloc] initWithWindow:this];
  
  KPWindow *native = [[KPWindow alloc]
    initWithRect:rect
        onScreen:this->output->screen
        delegate:this->view];

  native.title = [NSString stringWithCString:params->title
                                          encoding:NSUTF8StringEncoding];
  native.contentView = this->view;
  
  [native makeKeyAndOrderFront:nil];
  native.acceptsMouseMovedEvents = YES;
  return this;
}

