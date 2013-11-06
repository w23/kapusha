#include "X11Pointers.h"

namespace kapusha {
namespace sys {

static Pointers::Pointer::Flags xbutton_to_pointer(unsigned int button) {
  switch (button) {
  case Button1: return Pointers::Pointer::LeftButton;
  case Button2: return Pointers::Pointer::MiddleButton;
  case Button3: return Pointers::Pointer::RightButton;
  default: return Pointers::Pointer::None;
  }
}

bool X11Pointers::process(const XEvent &event) {
  switch (event.type) {
  case ButtonPress: {
      int btn = xbutton_to_pointer(event.xbutton.button);
      if (btn) evt_mouse_down(btn, event.xbutton.time);
      return true;
    }

  case ButtonRelease: {
      int btn = xbutton_to_pointer(event.xbutton.button);
      if (btn) evt_mouse_up(btn, event.xbutton.time);
      return true;
    }

  case MotionNotify:
    evt_mouse_move_to(vec2f(event.xmotion.x, event.xmotion.y), event.xmotion.time);
    return true;
  }
  return false;
}

} // namespace sys
} // namespace kapusha
