#import "KPWindow.h"

@interface KPWindow ()
- (void) commonInitializeWithRect:(NSRect)rect;
@end

@implementation KPWindow
- (instancetype)initWithWindow:(KP__cocoa_window_t*)window
  floatingParams:(const KPwindow_floating_params_t*)params
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
  [self setMinSize:NSSizeFromCGSize(
    CGSizeMake(params->min_width, params->min_height))];
  [self setMaxSize:NSSizeFromCGSize(
    CGSizeMake(params->max_width, params->max_height))];
  window_ = window;
  window_->output = 0;
  
  [self commonInitializeWithRect:rect];
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
  
  [self commonInitializeWithRect:
    NSMakeRect(0.0, 0.0, rect.size.width, rect.size.height)];
  return self;
}

- (void) commonInitializeWithRect:(NSRect)rect {
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

- (void) dealloc {
  [super dealloc];
}

-(BOOL) canBecomeKeyWindow {
	return YES;
}

// "@protocol" NSResponder

- (void) mouseMoved:(NSEvent *)theEvent {
}

// @protocol NSWindowDelegate
- (BOOL)windowShouldClose:(id)sender {
  if (window_->queue) {
    kpMessageQueuePut(window_->queue, window_->queue_tag, window_,
    KPWindowEventCloseRequest, 1, 0, 0);
    return NO;
  }
  return YES;
}

- (void)windowWillClose:(NSNotification *)notification {
  self.contentView = nil;
  kpMessageQueuePut(window_->queue, window_->queue_tag, window_,
    KPWindowEventClose, 1, 0, 0);
  window_->window = nil;
  kpWindowStop(window_);
}

// @protocol KPViewDelegate
- (void)viewWasInitialized {
  pevent_.type = KPWindowPaintBegin;
  window_->painter_func(&pevent_);
}

- (void)viewWasResized:(CGSize)size {
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

- (void)viewWillPresent:(const CVTimeStamp*)pts {
  pevent_.type = KPWindowPaintPaint;
  pevent_.time.pts = pts->videoTime;
  pevent_.time.time_delta = pts->videoRefreshPeriod;
  pevent_.time.time_delta_frame = pts->videoRefreshPeriod;
  window_->painter_func(&pevent_);
}

- (void)viewWillDisappear {
  pevent_.type = KPWindowPaintEnd;
  window_->painter_func(&pevent_);
}
@end

////////////////////////////////////////////////////////////////////////////////

void kp__CocoaWindowDtor(void *obj) {
  KP_THIS(KP__cocoa_window_t, obj);
  this->queue = 0;
  kpRelease(this->title);
  this->title = 0;
  
  kpWindowStop(this);
}

KPwindow_o kpWindowCreate(const KPwindow_create_params_t *params) {
  KP__cocoa_window_o this = KP_NEW(KP__cocoa_window_t, 0);
  this->O.dtor = kp__CocoaWindowDtor;
  this->user_data = params->paint_user_data;
  this->painter_func = params->painter;
  this->title = kpRetain(params->title);
  this->queue = params->queue;
  this->queue_tag = params->queue_tag;
  this->output = 0;
  this->window = nil;
  return this;
}

void kpWindowStart(KPwindow_o window,
  const KPwindow_floating_params_t *params)
{
  KP_THIS(KP__cocoa_window_t, window);
  KP_ASSERT(this->window == nil);
  
  this->window = [[KPWindow alloc] initWithWindow:this floatingParams:params];
  KP_ASSERT(this->window);
}

void kpWindowStartAttached(KPwindow_o window, KPoutput_video_o output) {
  KP_THIS(KP__cocoa_window_t, window);
  KP_ASSERT(this->window == nil);
  
  this->window = [[KPWindow alloc]
    initWithWindow: this
    output: (KP__cocoa_output_o)output];
  KP_ASSERT(this->window);
}

void kpWindowStop(KPwindow_o window) {
  KP_THIS(KP__cocoa_window_t, window);
  kpRelease(this->output);
  this->output = 0;

  if (this->window) {
    [this->window close];
    this->window = nil;
  }
}
