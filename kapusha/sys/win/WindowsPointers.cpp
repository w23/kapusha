#include "WindowsPointers.h"

namespace kapusha {
bool WindowsPointers::process_event(UINT msg, WPARAM wParam, LPARAM lParam, u32 now) {
  switch(msg) {
  case WM_LBUTTONDOWN:
    evt_mouse_down(Pointer::LeftButton, now);
    break;

  case WM_LBUTTONUP:
    evt_mouse_up(Pointer::LeftButton, now);
    break;

  case WM_MOUSEMOVE:
    if (ignore_) { ignore_ = false; break; }

    if (grabbed_) {
      evt_mouse_move_by(lparam_to_vec2f(lParam)-center_, now);
      grab();
    } else evt_mouse_move_to(lparam_to_vec2f(lParam), now);
    break;

  default:
    return false;
  }
  return true;
}

void WindowsPointers::resize(vec2i size) {
  size_ = size;
  evt_resize_viewport(vec2f(0.f, static_cast<float>(size.y)),
    vec2f(static_cast<float>(size.x), 0.f));
  center_ = vec2f(size_) * .5f;
}

void WindowsPointers::grab(bool grab_in, HWND window) {
  grabbed_ = grab_in;
  window_ = window;
  if (grabbed_) grab();
}
    
void WindowsPointers::grab() {
  POINT pt;
  pt.x = size_.x / 2;
  pt.y = size_.y / 2;
  ClientToScreen(window_, &pt);
  SetCursorPos(pt.x, pt.y);
  ignore_ = true;
}

} // namespace kapusha