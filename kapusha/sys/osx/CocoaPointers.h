#pragma once
#import <Cocoa/Cocoa.h>
#include <kapusha/viewport.h>
#include "MachTime.h"

namespace kapusha {
  class CocoaPointers : public Pointers {
  public:
    inline CocoaPointers() : relative_(false) {}
    inline void set_relative(bool relative) {
      relative_ = relative; }
    inline void resize(vec2f size) {
      evt_resize_viewport(vec2f(0.f), size);
    }
    void mouse_move_to(NSView *view, NSEvent *event) {
      if (!relative_)
        evt_mouse_move_to(location_in_view(view, event),
                          MachTime::sys_to_ms(event.timestamp));
      else
        evt_mouse_move_by(vec2f(event.deltaX, -event.deltaY),
                          MachTime::sys_to_ms(event.timestamp));
    }
    void mouse_down(NSView *view, NSEvent *event) {
      evt_mouse_down(Pointer::LeftButton,
                     MachTime::sys_to_ms(event.timestamp));
    }
    void mouse_up(NSView *view, NSEvent *event) {
      evt_mouse_up(Pointer::LeftButton,
                   MachTime::sys_to_ms(event.timestamp));
    }
  private:
    static vec2f location_in_view(NSView *view, NSEvent *event) {
      NSPoint pos = [view convertPoint:[event locationInWindow] fromView:nil];
      return vec2f(pos.x, pos.y);
    }
    bool relative_;
  }; // class CocoaPointerState
  
} // namespace kapusha