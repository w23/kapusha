#include "windows_inc.h"
#include <kapusha/core.h>
#include "WindowController.h"
#include "wstring.h"

namespace kapusha {
using sys::windows::WString;

WindowController::WindowController(HINSTANCE hInst, const IViewportFactory *factory)
  : viewport_(nullptr), need_redraw_(true) {
  const IViewportFactory::Preferences &prefs = factory->preferences();

  WNDCLASSEX wndclass = { 0 };
  wndclass.cbSize = sizeof wndclass;
  wndclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = window_proc;
  wndclass.hInstance = hInst;
  wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndclass.lpszClassName = L"KapushaWindowClass";
  RegisterClassEx(&wndclass);

  vec2i size = prefs.prefer_resolution;
  if (size.x <= 0 || size.y <= 0) size = vec2i(640, 480);

  RECT wr = {0, 0, size.x, size.y};
  AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

  window_ = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
                           wndclass.lpszClassName, WString(prefs.window_title),
                           WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW,
                           // | WS_VISIBLE, -- makes aero go awaY (?!?!)
                           CW_USEDEFAULT, CW_USEDEFAULT,
                           1, 1, // why not wr stuff? see MoveWindow below!11
                           NULL, NULL, hInst, this);

  context_ = new WGLContext(dc_ = GetDC(window_));
  context_->make_current();
  context_->load_procs();

  viewport_ = factory->create(this);
  //viewport_->resize(size);

  ShowWindow(window_, SW_SHOW);
  SetForegroundWindow(window_);
  SetFocus(window_);

  // WTF WINDOWS !!!!11
  // OTHERWISE THERE'S NO BORDER OR CAPTION ON FIRST APPEAR
  // AND CLIENT RECT IS BORKEN SO MOUSE POS IS ALSO INCORRECT
  // LE FOUCQUE
  MoveWindow(window_, 0, 0, wr.right-wr.left, wr.bottom-wr.top, TRUE);

  //if (wglSwapIntervalEXT) wglSwapIntervalEXT(1);

  //pointers_.resize(size);
}

WindowController::~WindowController() {
  delete viewport_;
  delete context_;
}

void WindowController::quit(int code) {
  PostQuitMessage(code);
}

LRESULT WindowController::process_event(UINT msg, WPARAM wParam, LPARAM lParam) {
  const u32 now = timer_.now();
  switch (msg) {
    //case WM_SIZING:
    case WM_SIZE: {
      //vec2i size(lParam & 0xffff, lParam >> 16);
      RECT rect;
      GetClientRect(window_, &rect);
      vec2i size(rect.right, rect.bottom);
      pointers_.resize(size);
      scale_ = math::vec2f(1.f) / math::vec2f(size);
      viewport_->resize(size);
	  break;
	  }

    case WM_KEYDOWN:
    case WM_KEYUP:
      if (keys_.process_event(msg, wParam, lParam, now))
        viewport_->in_keys(keys_);
      break;

    case WM_CLOSE:
    case WM_DESTROY:
  	  PostQuitMessage(0);
      break;

    case WM_QUIT: //! \fixme we don't get here
      break;

    default:
      if (pointers_.process_event(msg, wParam, lParam, now))
        viewport_->in_pointers(pointers_);
	  }
  return DefWindowProc(window_, msg, wParam, lParam);
}

LRESULT CALLBACK WindowController::window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  WindowController *winctl = reinterpret_cast<WindowController*>(
    GetWindowLongPtr(hWnd, GWLP_USERDATA));
  if (winctl) return winctl->process_event(msg, wParam, lParam);
  if (msg == WM_NCCREATE) {
    LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>(lParam);
    SetWindowLongPtr(hWnd, GWLP_USERDATA,
                     reinterpret_cast<LONG_PTR>(create_struct->lpCreateParams));
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WindowController::run() {
  MSG message;
  u32 prev = timer_.now();
  while (true) {
    if (need_redraw_) {
      while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
        if (message.message == WM_QUIT)
          return (int) message.wParam;
      }
    } else if (!GetMessage(&message, NULL, 0, 0)) break;
    //if (need_redraw_)
    {
      //need_redraw_ = false;
      const u32 now = timer_.now();
      viewport_->draw(now, (now - prev) / 1000.f);
      SwapBuffers(dc_);
      prev = now;
    }
  }
  return (int) message.wParam;
} // WindowController::run()
} // namespace kapusha
