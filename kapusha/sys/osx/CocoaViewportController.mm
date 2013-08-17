#include "CocoaViewportController.h"
#import "KPView_private.h"

namespace kapusha {
  CocoaViewportController::CocoaViewportController(KPView *view,
                                                   IViewport *viewport)
    : view_(view), viewport_(viewport), glContext_(nullptr) {
  }
  
  CocoaViewportController::~CocoaViewportController() {
    viewport_->close();
    delete glContext_;
  }
  
  void CocoaViewportController::quit(int code) {
    [view_.window performClose:nil];
  }
  
  void CocoaViewportController::requestRedraw() { [view_ requestRedraw]; }
  
  void CocoaViewportController::setTargetFramerate(int fps) {}
  
  void CocoaViewportController::setRelativeOnlyPointer(bool relative_only) {
    [view_ mouseAlwaysRelative:relative_only];
    pointerState_.setRelative(relative_only);
  }
  
  void CocoaViewportController::hideCursor(bool hide) {
    if (hide) CGDisplayHideCursor(kCGDirectMainDisplay);
    else CGDisplayShowCursor(kCGDirectMainDisplay);
  }
  
  const PointerState& CocoaViewportController::pointerState() const {
    return pointerState_;
  }
  
  const KeyState& CocoaViewportController::keyState() const {
    return keyState_;
  }
  
  void CocoaViewportController::init() {
    KP_ASSERT(!glContext_);
    glContext_ = new CocoaGLContext([view_ openGLContext]);
    viewport_->init(this, glContext_);
    time_.reset();
    prevFrameTime_ = 0;
  }
  
  void CocoaViewportController::resize(vec2i size) {
    viewport_->resize(size);
    pointerState_.resize(size);
  }
  
  void CocoaViewportController::draw() {
    u32 now = time_.now_ms();
    float dt = (now - prevFrameTime_) / 1000.f;
    viewport_->draw(now, dt);
    prevFrameTime_ = now;
  }
  
  void CocoaViewportController::mouseMoved(NSView *view, NSEvent *event) {
    pointerState_.mouseMoveTo(view, event);
    viewport_->inputPointer(pointerState_);
  }
  
  void CocoaViewportController::mouseDown(NSView *view, NSEvent *event) {
    pointerState_.mouseDown(view, event);
    viewport_->inputPointer(pointerState_);
  }
  
  void CocoaViewportController::mouseUp(NSView *view, NSEvent *event) {
    pointerState_.mouseUp(view, event);
    viewport_->inputPointer(pointerState_);
  }
  
  void CocoaViewportController::key(NSEvent* event, u32 time) {
    if (keyState_.processEvent(event, time)) viewport_->inputKey(keyState_);
  }
} // namespace kapusha