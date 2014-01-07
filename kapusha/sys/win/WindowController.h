#pragma once
#include <kapusha/viewport/IViewport.h>
#include "Timer.h"
#include "WindowsPointers.h"
#include "WindowsKeys.h"
#include "WGLContext.h"
#include "windows_inc.h"

namespace kapusha {

class WindowController : public IViewportController {
public:
  WindowController(HINSTANCE hInst, const IViewportFactory *factory);
  ~WindowController();

  virtual void quit(int code);
  virtual void grab_input(bool grab) {}
  virtual const Pointers& pointers() const { return pointers_; }
  virtual const Keys& keys() const { return keys_; }

  int run();

protected:
  static LRESULT CALLBACK window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  LRESULT process_event(UINT msg, WPARAM wParam, LPARAM lParam);

private:
  IViewport *viewport_;
  HWND window_;
  HDC dc_;
  math::vec2f scale_;
  bool need_redraw_;
  WindowsPointers pointers_;
  WindowsKeys keys_;
  WGLContext *context_;
  Timer timer_;
}; // class WindowController

} // namespace kapusha
