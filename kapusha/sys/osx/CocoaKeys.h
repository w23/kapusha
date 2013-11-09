#pragma once
#import <Cocoa/Cocoa.h>
#include <kapusha/viewport.h>

namespace kapusha {
  class CocoaKeys : public Keys {
  public:
    bool process_event(NSEvent* event, u32 time);
  private:
    static const int keymap_[128];
  };
} // namespace kapusha


