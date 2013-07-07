#pragma once
#import <Cocoa/Cocoa.h>
#include <kapusha/viewport.h>
#include "MachTime.h"

namespace kapusha {
  class CocoaPointerState : public PointerState {
  public:
    inline CocoaPointerState() : relative_(false) {}
    inline void setRelative(bool relative) { relative_ = relative; }
    inline void resize(vec2f size) {
      resizeViewport(vec2f(0.f), size);
    }
    void mouseMoveTo(NSView *view, NSEvent *event) {
      if (!relative_)
        PointerState::mouseMoveTo(locationInView(view, event),
                                  MachTime::sys_to_ms(event.timestamp));
      else
        PointerState::mouseMoveBy(vec2f(event.deltaX, -event.deltaY),
                                  MachTime::sys_to_ms(event.timestamp));
    }
    void mouseDown(NSView *view, NSEvent *event) {
      PointerState::mouseDown(Pointer::LeftButton,
                              MachTime::sys_to_ms(event.timestamp));
    }
    void mouseUp(NSView *view, NSEvent *event) {
      PointerState::mouseUp(Pointer::LeftButton,
                            MachTime::sys_to_ms(event.timestamp));
    }
  private:
    static vec2f locationInView(NSView *view, NSEvent *event) {
      NSPoint pos = [view convertPoint:[event locationInWindow] fromView:nil];
      return vec2f(pos.x, pos.y);
    }
    bool relative_;
  }; // class CocoaPointerState
  
} // namespace kapusha