#import "KPWindow.h"

@interface KPWindow ()
- (void) commonInitializeRect:(NSRect)rect;
@end

@implementation KPWindow
- (instancetype)initWithWindow:(KP__cocoa_window_t*)window
  freeParams:(const KPwindow_open_free_params_t*)params
{
  KP_ASSERT(window);
  KP_ASSERT(params);
  
  NSRect rect;
  rect.origin.x = rect.origin.y = 0;
  rect.size.width = (params->min_width + params->max_width) / 2;
  rect.size.height = (params->min_height + params->max_height) / 2;
  
  self = [super
    initWithContentRect: rect
    styleMask: NSTitledWindowMask | NSClosableWindowMask |
      NSMiniaturizableWindowMask | NSResizableWindowMask |
      NSTexturedBackgroundWindowMask
    backing: NSBackingStoreBuffered
    defer: YES
  ];
  KP_ASSERT(self);
  [self setMinSize:NSSizeFromCGSize(CGSizeMake(params->min_width, params->min_height))];
  [self setMaxSize:NSSizeFromCGSize(CGSizeMake(params->max_width, params->max_height))];
  window_ = window;
  window_->output = 0;
  
  [self commonInitializeRect:rect];
  return self;
}

- (instancetype)initWithWindow:(KP__cocoa_window_t*)window
  output:(KP__cocoa_output_o)output
{
  KP_ASSERT(output);
  NSRect rect = [output->screen frame];
  self = [[KPWindow alloc]
    initWithContentRect: rect
    styleMask: NSBorderlessWindowMask
    backing:NSBackingStoreBuffered
    defer:YES];
  KP_ASSERT(self);
  
  [self setLevel:NSMainMenuWindowLevel+1];
  [self setOpaque:YES];
  [self setHidesOnDeactivate:YES];
  
  window_ = window;
  window_->output = kpRetain(output);
  
  [self commonInitializeRect:
    NSMakeRect(0.0, 0.0, rect.size.width, rect.size.height)];
  return self;
}

- (void) commonInitializeRect:(NSRect)rect {
  pevent_.user_data = window_->user_data;
  pevent_.window = window_;

  self.title = [NSString stringWithCString:kpStringCString(window_->title)
    encoding:NSUTF8StringEncoding];
  self.delegate = self;

  KPView *view = [[KPView alloc] initWithSize:rect.size
    delegate:self];
  window_->window = self;

  [self setContentView:view];
  [self setAcceptsMouseMovedEvents:YES];
  [self makeKeyAndOrderFront:self];
}

-(BOOL) canBecomeKeyWindow {
	return YES;
}

- (void) mouseMoved:(NSEvent *)theEvent {
}

// @protocol NSWindowDelegate
- (BOOL)windowShouldClose:(id)sender {
  [[NSApplication sharedApplication] terminate:self];
  return YES;
}

- (void)windowWillClose:(NSNotification *)notification {
  [(KPView*)self.contentView willClose];
}

//@protocol KPViewDelegate
- (void)viewInitialized {
  pevent_.type = KPWindowPaintBegin;
  window_->painter_func(&pevent_);
}

- (void)viewResized:(CGSize)size {
  pevent_.type = KPWindowPaintReconfigure;
  pevent_.configuration.width = size.width;
  pevent_.configuration.height = size.height;
  
  if (window_->output)
    pevent_.configuration.aspect = size.width * window_->output->parent.hppmm
      / (size.height * window_->output->parent.vppmm);
  else
    pevent_.configuration.aspect = size.width / size.height;
  window_->painter_func(&pevent_);
}

- (void)viewPaint:(const CVTimeStamp*)pts {
  pevent_.type = KPWindowPaintPaint;
  pevent_.time.pts = pts->videoTime;
  pevent_.time.time_delta = pts->videoRefreshPeriod;
  pevent_.time.time_delta_frame = pts->videoRefreshPeriod;
  window_->painter_func(&pevent_);
}

- (void)viewStopping {
  pevent_.type = KPWindowPaintEnd;
  window_->painter_func(&pevent_);
}
@end

////////////////////////////////////////////////////////////////////////////////

void kp__CocoaWindowDtor(void *obj) {
  KP_THIS(KP__cocoa_window_t, obj);
  kpRelease(this->title);
  kpRelease(this->output);
  
  if (this->window) {
    [this->window close];
    this->window = nil;
  }
}

KPwindow_o kpWindowCreate(const KPwindow_create_params_t *params) {
  KP__cocoa_window_o this = KP_NEW(KP__cocoa_window_t, 0);
  this->O.dtor = kp__CocoaWindowDtor;
  this->user_data = params->paint_user_data;
  this->painter_func = params->painter;
  this->title = kpRetain(params->title);
  this->output = 0;
  this->window = nil;
  return this;
}

int kpWindowOpenFree(KPwindow_o window,
  const KPwindow_open_free_params_t *params)
{
  KP_THIS(KP__cocoa_window_t, window);
  KP_ASSERT(this->window == nil);
  
  this->window = [[KPWindow alloc] initWithWindow:this freeParams:params];
  
  return 1;
}

int kpWindowOpenAttached(KPwindow_o window, KPoutput_video_o output) {
  KP_THIS(KP__cocoa_window_t, window);
  KP_ASSERT(this->window == nil);
  
  this->window = [[KPWindow alloc]
    initWithWindow: this
    output: (KP__cocoa_output_o)output];
  
  return 1;

}
