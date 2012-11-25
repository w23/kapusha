#include <mach/mach_time.h>
#import "KPView.h"
#import "../../core/IViewport.h"

using namespace kapusha;

class CocoaViewportController;

@interface KPView ()
{
  CocoaViewportController *viewport_controller_;
  IViewport *viewport_;
  u32 prevFrameTime_;
  
  NSTimer *timer_;
  float frameTime_;
  //! \todo CVDisplayLinkRef displayLink_;
  // http://developer.apple.com/library/mac/#qa/qa1385/_index.html
  // why not now? threading! too lazy to think atm
}
@property (assign, nonatomic) BOOL warpMouse;
- (void) limitFps:(int)fps;

- (void) draw;

- (void) startDrawing;
- (void) stopDrawing;

- (void) doWarpMouse;
@end

////////////////////////////////////////////////////////////////////////////////

class MachTime {
public:
  MachTime()
  : start_(mach_absolute_time())
  {
    mach_timebase_info(&timebase_);
  }
  
  void reset()
  {
    start_ = mach_absolute_time();
  }
  
  u64 now_ns() const
  {
    uint64_t now = mach_absolute_time() - start_;
    return now * timebase_.numer / timebase_.denom;
  }
  
  u32 now_ms() const
  {
    return now_ns() / 1000000;
  }
private:
  uint64_t start_;
  mach_timebase_info_data_t timebase_;
};
MachTime g_machTime;

////////////////////////////////////////////////////////////////////////////////

class CocoaKeyState : public KeyState
{
public:
  bool processEvent(NSEvent* event, u32 time)
  {
    KP_ASSERT([event keyCode] < 128);
    return key(s_to_kapusha[[event keyCode]], event.type == NSKeyDown, time);
  }
  
private:
  static const int s_to_kapusha[128];
};

const int CocoaKeyState::s_to_kapusha[128] = {
  KeyA,       //0x00 = kVK_ANSI_A
  KeyS,       //0x01 = kVK_ANSI_S
  KeyD,       //0x02 = kVK_ANSI_D
  KeyF,       //0x03 = kVK_ANSI_F
  KeyH,       //0x04 = kVK_ANSI_H
  KeyG,       //0x05 = kVK_ANSI_G
  KeyZ,       //0x06 = kVK_ANSI_Z
  KeyX,       //0x07 = kVK_ANSI_X
  KeyC,       //0x08 = kVK_ANSI_C
  KeyV,       //0x09 = kVK_ANSI_V
  KeyUnknown, //0x0A = kVK_ISO_Section
  KeyB,       //0x0B = kVK_ANSI_B
  KeyQ,       //0x0C = kVK_ANSI_Q
  KeyW,       //0x0D = kVK_ANSI_W
  KeyE,       //0x0E = kVK_ANSI_E
  KeyR,       //0x0F = kVK_ANSI_R
  KeyY,       //0x10 = kVK_ANSI_Y
  KeyT,       //0x11 = kVK_ANSI_T
  Key1,       //0x12 = kVK_ANSI_1
  Key2,       //0x13 = kVK_ANSI_2
  Key3,       //0x14 = kVK_ANSI_3
  Key4,       //0x15 = kVK_ANSI_4
  Key6,       //0x16 = kVK_ANSI_6
  Key5,       //0x17 = kVK_ANSI_5
  KeyEqual,   //0x18 = kVK_ANSI_Equal
  Key9,       //0x19 = kVK_ANSI_9
  Key7,       //0x1A = kVK_ANSI_7
  KeyMinus,   //0x1B = kVK_ANSI_Minus
  Key8,       //0x1C = kVK_ANSI_8
  Key0,       //0x1D = kVK_ANSI_0
  KeyUnknown, //0x1E = kVK_ANSI_RightBracket
  KeyO,       //0x1F = kVK_ANSI_O
  KeyU,       //0x20 = kVK_ANSI_U
  KeyUnknown, //0x21 = kVK_ANSI_LeftBracket
  KeyI,       //0x22 = kVK_ANSI_I
  KeyP,       //0x23 = kVK_ANSI_P
  KeyEnter,   //0x24 = kVK_Return
  KeyL,       //0x25 = kVK_ANSI_L
  KeyJ,       //0x26 = kVK_ANSI_J
  KeyUnknown, //0x27 = kVK_ANSI_Quote
  KeyK,       //0x28 = kVK_ANSI_K
  KeyUnknown, //0x29 = kVK_ANSI_Semicolon
  KeyUnknown, //0x2A = kVK_ANSI_Backslash
  KeyComma,   //0x2B = kVK_ANSI_Comma
  KeySlash,   //0x2C = kVK_ANSI_Slash
  KeyN,       //0x2D = kVK_ANSI_N
  KeyM,       //0x2E = kVK_ANSI_M
  KeyDot,     //0x2F = kVK_ANSI_Period
  KeyTab,     //0x30 = kVK_Tab
  KeySpace,   //0x31 = kVK_Space
  KeyUnknown, //0x32 = kVK_ANSI_Grave
  KeyDel,     //0x33 = kVK_Delete
  KeyUnknown, //0x34
  KeyEsc,     //0x35 = kVK_Escape
  KeyUnknown, //0x36
  KeyUnknown, //0x37 = kVK_Command
  KeyShift,   //0x38 = kVK_Shift
  KeyUnknown, //0x39 = kVK_CapsLock
  KeyOpt,     //0x3A = kVK_Option
  KeyCtrl,    //0x3B = kVK_Control
  KeyUnknown, //0x3C = kVK_RightShift
  KeyUnknown, //0x3D = kVK_RightOption
  KeyUnknown, //0x3E = kVK_RightControl
  KeyUnknown, //0x3F = kVK_Function
  KeyUnknown, //0x40 = kVK_F17
  KeyUnknown, //0x41 = kVK_ANSI_KeypadDecimal
  KeyUnknown, //0x42
  KeyUnknown, //0x43 = kVK_ANSI_KeypadMultiply
  KeyUnknown, //0x44
  KeyUnknown, //0x45 = kVK_ANSI_KeypadPlus
  KeyUnknown, //0x46
  KeyUnknown, //0x47 = kVK_ANSI_KeypadClear
  KeyUnknown, //0x48 = kVK_VolumeUp
  KeyUnknown, //0x49 = kVK_VolumeDown
  KeyUnknown, //0x4A = kVK_Mute
  KeyUnknown, //0x4B = kVK_ANSI_KeypadDivide
  KeyUnknown, //0x4C = kVK_ANSI_KeypadEnter
  KeyUnknown, //0x4D
  KeyUnknown, //0x4E = kVK_ANSI_KeypadMinus
  KeyUnknown, //0x4F = kVK_F18
  KeyUnknown, //0x50 = kVK_F19
  KeyUnknown, //0x51 = kVK_ANSI_KeypadEquals
  KeyUnknown, //0x52 = kVK_ANSI_Keypad0
  KeyUnknown, //0x53 = kVK_ANSI_Keypad1
  KeyUnknown, //0x54 = kVK_ANSI_Keypad2
  KeyUnknown, //0x55 = kVK_ANSI_Keypad3
  KeyUnknown, //0x56 = kVK_ANSI_Keypad4
  KeyUnknown, //0x57 = kVK_ANSI_Keypad5
  KeyUnknown, //0x58 = kVK_ANSI_Keypad6
  KeyUnknown, //0x59 = kVK_ANSI_Keypad7
  KeyUnknown, //0x5A = kVK_F20
  KeyUnknown, //0x5B = kVK_ANSI_Keypad8
  KeyUnknown, //0x5C = kVK_ANSI_Keypad9
  KeyUnknown, //0x5D = kVK_JIS_Yen
  KeyUnknown, //0x5E = kVK_JIS_Underscore
  KeyUnknown, //0x5F = kVK_JIS_KeypadComma
  KeyUnknown, //0x60 = kVK_F5
  KeyUnknown, //0x61 = kVK_F6
  KeyUnknown, //0x62 = kVK_F7
  KeyUnknown, //0x63 = kVK_F3
  KeyUnknown, //0x64 = kVK_F8
  KeyUnknown, //0x65 = kVK_F9
  KeyUnknown, //0x66 = kVK_JIS_Eisu
  KeyUnknown, //0x67 = kVK_F11
  KeyUnknown, //0x68 = kVK_JIS_Kana
  KeyUnknown, //0x69 = kVK_F13
  KeyUnknown, //0x6A = kVK_F16
  KeyUnknown, //0x6B = kVK_F14
  KeyUnknown, //0x6C
  KeyUnknown, //0x6D = kVK_F10
  KeyUnknown, //0x6E
  KeyUnknown, //0x6F = kVK_F12
  KeyUnknown, //0x71 = kVK_F15
  KeyUnknown, //0x72 = kVK_Help
  KeyUnknown, //0x73 = kVK_Home
  KeyPageUp,  //0x74 = kVK_PageUp
  KeyUnknown, //0x75 = kVK_ForwardDelete
  KeyUnknown, //0x76 = kVK_F4
  KeyUnknown, //0x77 = kVK_End
  KeyUnknown, //0x78 = kVK_F2
  KeyPageDown,//0x79 = kVK_PageDown
  KeyUnknown, //0x7A = kVK_F1
  KeyLeft,    //0x7B = kVK_LeftArrow
  KeyRight,   //0x7C = kVK_RightArrow
  KeyDown,    //0x7D = kVK_DownArrow
  KeyUp,      //0x7E = kVK_UpArrow
  KeyUnknown  //0x7F
};

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
  virtual void limitFramesPerSecond(int fps)
  {
    [view_ limitFps:fps];
  }
  
  virtual void limitlessPointer(bool limitless)
  {
    view_.warpMouse = limitless;
  }
  
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
  
  inline void key(NSEvent* event, u32 time)
  {
    if (keyState_.processEvent(event, time))
      viewport_->inputKey(keyState_);
  }
    
private:
  KPView *view_;
  IViewport *viewport_;
  PointerState pointerState_;
  CocoaKeyState keyState_;
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
    [self draw];

    if (frameTime_ != 0)
      [self startDrawing];
  }
}

- (void) limitFps:(int)fps
{
  if (fps > 0)
  {
    frameTime_ = 1.f / (float)fps;
    [self startDrawing];
  } else {
    [self stopDrawing];
  }
}

- (void) startDrawing
{
  if (timer_) [timer_ invalidate];
  timer_ = [NSTimer scheduledTimerWithTimeInterval:frameTime_
                                            target:self
                                          selector:@selector(timerDraw)
                                          userInfo:nil
                                           repeats:YES];
}

- (void) stopDrawing
{
  [timer_ invalidate];
  timer_ = nil;
}

- (void) timerDraw
{
  [self draw];
}

- (void) draw
{
  if (viewport_)
  {
    u32 time = g_machTime.now_ms();
    viewport_->draw(time, (time - prevFrameTime_) / 1000.f);
    [[self openGLContext] flushBuffer];
    prevFrameTime_ = time;
  }
}

 
#pragma mark - delegate methods

- (void) prepareOpenGL
{
  g_machTime.reset();
  prevFrameTime_ = g_machTime.now_ms();
  
  delete viewport_controller_;
  viewport_controller_ = 0;
  
  [self limitFps:60];
  
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
  [self draw];
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

- (void) doWarpMouse
{
  KP_ASSERT(!"warping mouse is broken!");
  /* \fixme
  NSPoint point;
  point.x = self.bounds.size.width / 2.;
  point.y = self.bounds.size.height / 2.;
  NSPoint windowPoint = [self convertPoint:point toView:nil];
  NSPoint screenPoint = [[self window] convertBaseToScreen:windowPoint];
  CGWarpMouseCursorPosition(screenPoint);
   */
}

#pragma mark Mouse events

- (void) mouseMoved:(NSEvent *)theEvent
{
  viewport_controller_->mouseMoved([self viewportMouseLocation:theEvent],
                                   [self viewportTime:[theEvent timestamp]]);
  if (_warpMouse) [self doWarpMouse];
}

- (void) mouseDragged:(NSEvent *)theEvent
{
  viewport_controller_->mouseMoved([self viewportMouseLocation:theEvent],
                                   [self viewportTime:[theEvent timestamp]]);
  if (_warpMouse) [self doWarpMouse];
}

- (void) mouseDown:(NSEvent *)theEvent
{
  viewport_controller_->mouseClicked([self viewportMouseLocation:theEvent],
                                     PointerState::Pointer::LeftButton,
                                     [self viewportTime:[theEvent timestamp]]);
  if (_warpMouse) [self doWarpMouse];
}

- (void) mouseUp:(NSEvent *)theEvent
{
  viewport_controller_->mouseUnclicked([self viewportMouseLocation:theEvent],
                                       PointerState::Pointer::LeftButton,
                                       [self viewportTime:[theEvent timestamp]]);
  if (_warpMouse) [self doWarpMouse];
}

//! \todo right, middle buttons, wheel

#pragma mark Key events

- (void) keyDown:(NSEvent *)theEvent
{
  if (!theEvent.isARepeat)
    viewport_controller_->key(theEvent, [self viewportTime:theEvent.timestamp]);
}

- (void) flagsChanged:(NSEvent *)theEvent
{
  viewport_controller_->key(theEvent, [self viewportTime:theEvent.timestamp]);
}

- (void) keyUp:(NSEvent *)theEvent
{
  viewport_controller_->key(theEvent, [self viewportTime:theEvent.timestamp]);
}


@end
