#import "KPView.h"
#import "../../core/IViewport.h"

using namespace kapusha;

class CocoaViewportController;

@interface KPView ()
{
  CocoaViewportController *viewport_controller_;
  IViewport *viewport_;
  NSTimer *timer_;
  BOOL redraw_;
}
- (void) redraw;
@end

////////////////////////////////////////////////////////////////////////////////

class CocoaViewportController : public IViewportController
{
public:
  CocoaViewportController(KPView *view, IViewport *viewport)
  : view_(view), viewport_(viewport)
  {
    viewport_->init(this);
  }
  
  CocoaViewportController()
  {
    viewport_->close();
  }
  
public: // IViewportController
  virtual void quit(int code) {}
  virtual void redraw() {
    [view_ redraw];
  }
  virtual void pointerReset(bool reset) {}
  virtual const PointerState& pointerState() const { return pointerState_; }
  virtual const KeyState& keyState() const { return keyState_; }
    
//
  inline void mouseMoved(vec2f pos, u32 time)
  {
    pointerState_.mouseMove(pos, time);
    viewport_->inputPointer(pointerState_);
  }
  
  inline void mouseClicked(vec2f pos, int button, u32 time)
  {
    pointerState_.mouseClick(pos, button, time);
  }
  
  inline void mouseUnclicked(vec2f pos, int button, u32 time)
  {
    pointerState_.mouseUnclick(pos, button, time);
  }
    
private:
  KPView *view_;
  IViewport *viewport_;
  PointerState pointerState_;
  KeyState keyState_;
};

////////////////////////////////////////////////////////////////////////////////

@implementation KPView

- (void) dealloc
{
  delete viewport_controller_;
  [super dealloc];
}

- (void) setViewport:(kapusha::IViewport*)viewport
{
  viewport_ = viewport;
  
  if ([self openGLContext])
  {
    [self prepareOpenGL];
    [self reshape];
    [self drawRect:[self bounds]];
    timer_ = [NSTimer timerWithTimeInterval:(1.0f/60.0f)
                                     target:self
                                   selector:@selector(timer:)
                                   userInfo:nil
                                    repeats:YES];
	[[NSRunLoop currentRunLoop] addTimer:timer_ forMode:NSDefaultRunLoopMode];
  }
}

- (void) redraw
{
  redraw_ = YES;
}

- (void) timer:(id)obj
{
  [self drawRect:[self bounds]];
}

#pragma mark - delegate methods

- (void) prepareOpenGL
{
  delete viewport_controller_;
  viewport_controller_ = 0;
  
  if (viewport_)
    viewport_controller_ = new CocoaViewportController(self, viewport_);
}

- (void) reshape
{
  if (viewport_)
    viewport_->resize(kapusha::vec2i(self.bounds.size.width,
                                     self.bounds.size.height));
}

- (void)drawRect:(NSRect)dirtyRect
{
  if (viewport_)
  {
    redraw_ = NO;
    viewport_->draw(1.f, .1f);
    [[self openGLContext] flushBuffer];
  }
}

- (BOOL)acceptsFirstResponder {
  return YES;
}
- (BOOL)becomeFirstResponder {
  return  YES;
}
- (BOOL)resignFirstResponder {
  return YES;
}

#pragma mark Mouse events

- (vec2f) viewportMouseLocation:(NSEvent *)event
{
  NSPoint pos = [self convertPoint:[event locationInWindow] fromView:nil];
  vec2f ret(pos.x, pos.y);
  vec2f size(self.bounds.size.width, self.bounds.size.height);
  return (ret / size) * 2.f - 1.f;
}

- (u32) viewportTime:(NSTimeInterval)cocoaTime
{
  return (u32)cocoaTime * 1000;
}

- (void) mouseMoved:(NSEvent *)theEvent
{
  viewport_controller_->mouseMoved([self viewportMouseLocation:theEvent],
                                   [self viewportTime:[theEvent timestamp]]);
}

- (void) mouseDragged:(NSEvent *)theEvent
{
  viewport_controller_->mouseMoved([self viewportMouseLocation:theEvent],
                                   [self viewportTime:[theEvent timestamp]]);
}

- (void) mouseDown:(NSEvent *)theEvent
{
  viewport_controller_->mouseClicked([self viewportMouseLocation:theEvent],
                                     PointerState::Pointer::LeftButton,
                                     [self viewportTime:[theEvent timestamp]]);
}

- (void) mouseUp:(NSEvent *)theEvent
{
  viewport_controller_->mouseUnclicked([self viewportMouseLocation:theEvent],
                                       PointerState::Pointer::LeftButton,
                                       [self viewportTime:[theEvent timestamp]]);
}

//! \todo right, middle buttons, wheel

#pragma mark Key events

- (void) keyUp:(NSEvent *)theEvent
{
  
}

- (void) keyDown:(NSEvent *)theEvent
{
  
}

- (void) flagsChanged:(NSEvent *)theEvent
{
  
}

@end
