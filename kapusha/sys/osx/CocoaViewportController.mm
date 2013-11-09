#include "CocoaViewportController.h"
#import "KPView_private.h"

namespace kapusha {
  CocoaViewportController::CocoaViewportController(KPView *view,
                                                   const IViewportFactory *factory)
    : view_(view), factory_(factory), context_(nullptr) {
  }
  
  CocoaViewportController::~CocoaViewportController() {
  }
  
  void CocoaViewportController::quit(int code) {
    [view_.window performClose:nil];
  }
  
  void CocoaViewportController::grab_input(bool grab) {
    if (grab) CGDisplayHideCursor(kCGDirectMainDisplay);
    else CGDisplayShowCursor(kCGDirectMainDisplay);

    [view_ mouseAlwaysRelative:grab];
    pointers_.set_relative(grab);
  }
  
  const Pointers& CocoaViewportController::pointers() const {
    return pointers_;
  }
  
  const Keys& CocoaViewportController::keys() const {
    return keys_;
  }
  
  void CocoaViewportController::init() {
    KP_ASSERT(!context_);
    KP_ASSERT(!viewport_);
    context_.reset(new CocoaGLContext([view_ openGLContext]));
    context_->make_current();
    viewport_.reset(factory_->create(this));
    time_.reset();
    prevFrameTime_ = 0;
  }
  
  void CocoaViewportController::resize(vec2i size) {
    KP_ASSERT(viewport_);
    viewport_->resize(size);
    pointers_.resize(size);
  }
  
  void CocoaViewportController::draw() {
    KP_ASSERT(viewport_);
    u32 now = time_.now_ms();
    float dt = (now - prevFrameTime_) / 1000.f;
    viewport_->draw(now, dt);
    prevFrameTime_ = now;
  }
  
  void CocoaViewportController::mouse_moved(NSView *view, NSEvent *event) {
    KP_ASSERT(viewport_);
    pointers_.mouse_move_to(view, event);
    viewport_->in_pointers(pointers_);
  }
  
  void CocoaViewportController::mouse_down(NSView *view, NSEvent *event) {
    KP_ASSERT(viewport_);
    pointers_.mouse_down(view, event);
    viewport_->in_pointers(pointers_);
  }
  
  void CocoaViewportController::mouse_up(NSView *view, NSEvent *event) {
    KP_ASSERT(viewport_);
    pointers_.mouse_up(view, event);
    viewport_->in_pointers(pointers_);
  }
  
  void CocoaViewportController::key(NSEvent* event, u32 time) {
    KP_ASSERT(viewport_);
    if (keys_.process_event(event, time)) viewport_->in_keys(keys_);
  }
} // namespace kapusha