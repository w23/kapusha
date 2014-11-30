#import "KPWindow.h"

#define KP__SYS "Cocoa::window"

@interface KPWindow ()
- (void) renderStart;
- (void) renderStop;
- (void) renderRepaint;
@end

@implementation KPWindow
- (instancetype)
  initWithParams:(const KPwindow_create_params_t*)params
  window:(KP__cocoa_window_t*)window
{
  KP_ASSERT(params);
  KP_ASSERT(window);
  
  NSRect rect;
  NSUInteger style_mask;
  
  if (params->attachment) {
    KP__cocoa_output_t *output = (KP__cocoa_output_t*)params->attachment;
    rect = [output->screen frame];
    style_mask = NSBorderlessWindowMask;
  } else {
    NSScreen *screen = [NSScreen mainScreen];
    rect.size.width = screen.frame.size.width / 2;
    rect.size.height = screen.frame.size.height / 2;
    rect.origin.x = rect.size.width / 2;
    rect.origin.y = rect.size.height / 2;
    style_mask = NSTitledWindowMask | NSClosableWindowMask |
      NSMiniaturizableWindowMask | NSResizableWindowMask |
      NSTexturedBackgroundWindowMask;
  }
  
  self = [super
    initWithContentRect: rect
    styleMask: style_mask
    backing: NSBackingStoreBuffered
    defer: YES
  ];
  KP_ASSERT(self);
  
  if (params->attachment) {
    [self setLevel:NSMainMenuWindowLevel+1];
    [self setOpaque:YES];
    [self setHidesOnDeactivate:YES];
  } else {
    [self setMinSize:NSSizeFromCGSize(CGSizeMake(64, 64))];
    /* ?? [self setMaxSize:NSSizeFromCGSize(max)]; */
  }
  window_ = window;
  output_ = kpRetain(params->attachment);
  queue_seq_ = 0;
  queue_ = params->queue;
  queue_userdata_ = params->queue_userdata;
  
  painter_ = params->painter;
  pevent_.user_data = params->paint_user_data;
  pevent_.window = window_;

  self.title = [NSString stringWithCString:kpStringCString(params->title)
    encoding:NSUTF8StringEncoding];
  self.delegate = self;

  KPView *view = [[KPView alloc] initWithSize:rect.size delegate:self];
  window_->window = self;

  [self setContentView:view];
  [self setAcceptsMouseMovedEvents:YES];
  [self makeKeyAndOrderFront:self];
  return self;
}

- (void) dealloc {
  [super dealloc];
}

-(BOOL) canBecomeKeyWindow {
	return YES;
}

- (void) processMouseEvent:(NSEvent*)event {
}

- (void) processKeyboardEvent:(NSEvent*)event {
}

// "@protocol" NSResponder

- (void) mouseDown:(NSEvent *)theEvent {
  [self processMouseEvent:theEvent];
}

- (void) mouseUp:(NSEvent *)theEvent {
  [self processMouseEvent:theEvent];
}

- (void) mouseMoved:(NSEvent *)theEvent {
  [self processMouseEvent:theEvent];
}

- (void) keyDown:(NSEvent *)theEvent {
  [self processKeyboardEvent:theEvent];
}

- (void) keyUp:(NSEvent *)theEvent {
  [self processKeyboardEvent:theEvent];
}

- (void) flagsChanged:(NSEvent *)theEvent {
  [self processKeyboardEvent:theEvent];
}

// @protocol NSWindowDelegate
- (BOOL)windowShouldClose:(id)sender {
  if (!queue_) return YES;
  
  /* TODO pool */
  KPmessage_t msg;
  msg.timestamp = kpSysTimeNs();
  msg.sequence = queue_seq_++;
  msg.origin = window_;
  msg.user = queue_userdata_;
  msg.type = KPWindowEventCloseRequest;
  msg.param = 0;
  msg.data = 0;
  msg.size = 0;
  kpMessageQueuePutCopy(queue_, &msg);
  return NO;
}

- (void)windowWillClose:(NSNotification *)notification {
  [(KPView*)self.contentView renderStop];
  self.contentView = nil;
  
  if (queue_) {
    KPmessage_t msg;
    msg.timestamp = kpSysTimeNs();
    msg.sequence = queue_seq_++;
    msg.origin = window_;
    msg.user = queue_userdata_;
    msg.type = KPWindowEventDestroyed;
    msg.param = 0;
    msg.data = 0;
    msg.size = 0;
    kpMessageQueuePutCopy(queue_, &msg);
  }
  window_ = 0;
}

// @protocol KPViewDelegate
- (void)viewWasInitialized {
  pevent_.type = KPWindowPaintBegin;
  painter_(&pevent_);
}

- (void)viewWasResized:(CGSize)size {
  pevent_.type = KPWindowPaintReconfigure;
  pevent_.configuration.width = size.width;
  pevent_.configuration.height = size.height;
  
  if (output_)
    pevent_.configuration.aspect = size.width * output_->parent.hppmm
      / (size.height * output_->parent.vppmm);
  else
    pevent_.configuration.aspect = size.width / size.height;
  painter_(&pevent_);
}

- (void)viewWillPresent:(const CVTimeStamp*)pts {
  pevent_.type = KPWindowPaintPaint;
  pevent_.time.pts = pts->videoTime;
  pevent_.time.time_delta = pts->videoRefreshPeriod;
  pevent_.time.time_delta_frame = pts->videoRefreshPeriod;
  painter_(&pevent_);
}

- (void)viewWillDisappear {
  pevent_.type = KPWindowPaintEnd;
  painter_(&pevent_);
}

- (void) renderStart {
  [self.contentView renderStart];
}

- (void) renderStop {
  [self.contentView renderStop];
}

- (void) renderRepaint {
  [self.contentView renderRepaint];
}
@end

////////////////////////////////////////////////////////////////////////////////

// there's only way to get to this function -- via user release
void kp__CocoaWindowDtor(void *obj) {
  KP_ASSERT(obj);
  KP_THIS(KP__cocoa_window_t, obj);
  [this->window
    performSelectorOnMainThread:@selector(close)
    withObject:nil
    waitUntilDone:YES];
}

KPwindow_o kpWindowCreate(const KPwindow_create_params_t *params) {
  __block KPwindow_o retval;
  dispatch_sync(dispatch_get_main_queue(), ^(){
    KP__cocoa_window_o this = KP_NEW(KP__cocoa_window_t, 0);
    this->O.dtor = kp__CocoaWindowDtor;
    this->window = [[KPWindow alloc]
      initWithParams:(const KPwindow_create_params_t*)params
      window:this];
    KP_ASSERT(this->window);
    retval = this;
  });
  return retval;
}

void kpWindowStart(KPwindow_o window) {
  KP_ASSERT(window);
  KP_THIS(KP__cocoa_window_t, window);
  KP_ASSERT(this->window);
  [this->window
    performSelectorOnMainThread:@selector(renderStart)
    withObject:nil
    waitUntilDone:YES];
}

void kpWindowRepaint(KPwindow_o window) {
  KP_ASSERT(window);
  KP_THIS(KP__cocoa_window_t, window);
  KP_ASSERT(this->window);
  [this->window
    performSelectorOnMainThread:@selector(renderRepaint)
    withObject:nil
    waitUntilDone:YES];
}

void kpWindowStop(KPwindow_o window) {
  KP_ASSERT(window);
  KP_THIS(KP__cocoa_window_t, window);
  KP_ASSERT(this->window);
  [this->window
    performSelectorOnMainThread:@selector(renderStop)
    withObject:nil
    waitUntilDone:YES];
}
