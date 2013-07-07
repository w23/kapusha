#pragma once
#import <Cocoa/Cocoa.h>
#include <kapusha/viewport.h>

namespace kapusha {
  class CocoaKeyState : public KeyState {
  public:
    bool processEvent(NSEvent* event, u32 time);
  private:
    static const int keymap[128];
  };
} // namespace kapusha


