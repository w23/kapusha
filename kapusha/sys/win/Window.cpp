#include <windows.h>
#include <windowsx.h>
#include <kapusha/core/Log.h>
#include <kapusha/core/IViewport.h>
#include <kapusha/render/OpenGL.h>

namespace kapusha {

///////////////////////////////////////////////////////////////////////////////
  class WindowsKeyState : public KeyState
  {
  public:
    bool processEvent(UINT msg, WPARAM wParam, LPARAM lParam);
    static const int winkey_to_kapusha_[255];
  };

  const int WindowsKeyState::winkey_to_kapusha_[255] = {
    KeyUnknown,
    KeyUnknown, // VK_LBUTTON        0x01
    KeyUnknown, // VK_RBUTTON        0x02
    KeyUnknown, // VK_CANCEL         0x03
    KeyUnknown, // VK_MBUTTON        0x04
    KeyUnknown, // VK_XBUTTON1       0x05
    KeyUnknown, // VK_XBUTTON2       0x06
    KeyUnknown, //                   0x07 ???
    KeyUnknown, // VK_BACK           0x08
    KeyTab,     // VK_TAB            0x09
    KeyUnknown, //                   0x0A ???
    KeyUnknown, //                   0x0B ???
    KeyUnknown, // VK_CLEAR          0x0C
    KeyEnter,   // VK_RETURN         0x0D
    KeyUnknown, //                   0x0E ???
    KeyUnknown, //                   0x0F ???
    //! \fixme
    KeyLeftShift, // VK_SHIFT          0x10
    KeyLeftCtrl,  // VK_CONTROL        0x11
    KeyLeftAlt,   // VK_MENU           0x12
    KeyUnknown, // VK_PAUSE          0x13
    KeyUnknown, // VK_CAPITAL        0x14
    KeyUnknown, // VK_HANGUL         0x15 also: VK_KANA
    KeyUnknown, //                   0x16 ???
    KeyUnknown, // VK_JUNJA          0x17
    KeyUnknown, // VK_FINAL          0x18
    KeyUnknown, // VK_KANJI          0x19
    KeyUnknown, //                   0x1A ???
    KeyEsc,     // VK_ESCAPE         0x1B
    KeyUnknown, // VK_CONVERT        0x1C
    KeyUnknown, // VK_NONCONVERT     0x1D
    KeyUnknown, // VK_ACCEPT         0x1E
    KeyUnknown, // VK_MODECHANGE     0x1F
    KeySpace,   // VK_SPACE          0x20
    KeyUnknown, // VK_PRIOR          0x21
    KeyUnknown, // VK_NEXT           0x22
    KeyUnknown, // VK_END            0x23
    KeyUnknown, // VK_HOME           0x24
    KeyLeft,    // VK_LEFT           0x25
    KeyUp,      // VK_UP             0x26
    KeyRight,   // VK_RIGHT          0x27
    KeyDown,    // VK_DOWN           0x28
    KeyUnknown, // VK_SELECT         0x29
    KeyUnknown, // VK_PRINT          0x2A
    KeyUnknown, // VK_EXECUTE        0x2B
    KeyUnknown, // VK_SNAPSHOT       0x2C
    KeyUnknown, // VK_INSERT         0x2D
    KeyDel,     // VK_DELETE         0x2E
    KeyUnknown, // VK_HELP           0x2F
    Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9,
    KeyUnknown, //                   0x3A ???
    KeyUnknown, //                   0x3B ???
    KeyUnknown, //                   0x3C ???
    KeyUnknown, //                   0x3D ???
    KeyUnknown, //                   0x3E ???
    KeyUnknown, //                   0x3F ???
    KeyUnknown, //                   0x40 ???
    KeyA, KeyB, KeyC, KeyD, KeyE, KeyF, KeyG, KeyH, KeyI, KeyJ, KeyK, KeyL,
    KeyM, KeyN, KeyO, KeyP, KeyQ, KeyR, KeyS, KeyT, KeyU, KeyV, KeyW, KeyX,
    KeyY, KeyZ,
    KeyUnknown, // VK_LWIN           0x5B
    KeyUnknown, // VK_RWIN           0x5C
    KeyUnknown, // VK_APPS           0x5D
    KeyUnknown, //                   0x5E ???
    KeyUnknown, // VK_SLEEP          0x5F
    KeyUnknown, // VK_NUMPAD0        0x60
    KeyUnknown, // VK_NUMPAD1        0x61
    KeyUnknown, // VK_NUMPAD2        0x62
    KeyUnknown, // VK_NUMPAD3        0x63
    KeyUnknown, // VK_NUMPAD4        0x64
    KeyUnknown, // VK_NUMPAD5        0x65
    KeyUnknown, // VK_NUMPAD6        0x66
    KeyUnknown, // VK_NUMPAD7        0x67
    KeyUnknown, // VK_NUMPAD8        0x68
    KeyUnknown, // VK_NUMPAD9        0x69
    KeyUnknown, // VK_MULTIPLY       0x6A
    KeyUnknown, // VK_ADD            0x6B
    KeyUnknown, // VK_SEPARATOR      0x6C
    KeyUnknown, // VK_SUBTRACT       0x6D
    KeyUnknown, // VK_DECIMAL        0x6E
    KeyUnknown, // VK_DIVIDE         0x6F
    KeyF1,      // VK_F1             0x70
    KeyF2,      // VK_F2             0x71
    KeyF3,      // VK_F3             0x72
    KeyF4,      // VK_F4             0x73
    KeyF5,      // VK_F5             0x74
    KeyF6,      // VK_F6             0x75
    KeyF7,      // VK_F7             0x76
    KeyF8,      // VK_F8             0x77
    KeyF9,      // VK_F9             0x78
    KeyF10,     // VK_F10            0x79
    KeyF11,     // VK_F11            0x7A
    KeyF12,     // VK_F12            0x7B
    KeyUnknown, // VK_F13            0x7C
    KeyUnknown, // VK_F14            0x7D
    KeyUnknown, // VK_F15            0x7E
    KeyUnknown, // VK_F16            0x7F
    KeyUnknown, // VK_F17            0x80
    KeyUnknown, // VK_F18            0x81
    KeyUnknown, // VK_F19            0x82
    KeyUnknown, // VK_F20            0x83
    KeyUnknown, // VK_F21            0x84
    KeyUnknown, // VK_F22            0x85
    KeyUnknown, // VK_F23            0x86
    KeyUnknown, // VK_F24            0x87
    KeyUnknown, //                   0x88 ???
    KeyUnknown, //                   0x89 ???
    KeyUnknown, //                   0x8A ???
    KeyUnknown, //                   0x8B ???
    KeyUnknown, //                   0x8C ???
    KeyUnknown, //                   0x8D ???
    KeyUnknown, //                   0x8E ???
    KeyUnknown, //                   0x8F ???
    KeyUnknown, // VK_NUMLOCK        0x90
    KeyUnknown, // VK_SCROLL         0x91
    KeyUnknown, // VK_OEM_NEC_EQUAL  0x92   // '=' key on numpad
    KeyUnknown, // VK_OEM_FJ_MASSHOU 0x93
    KeyUnknown, // VK_OEM_FJ_TOUROKU 0x94
    KeyUnknown, // VK_OEM_FJ_LOYA    0x95
    KeyUnknown, // VK_OEM_FJ_ROYA    0x96
    KeyUnknown, //                   0x97 ???
    KeyUnknown, //                   0x98 ???
    KeyUnknown, //                   0x99 ???
    KeyUnknown, //                   0x9A ???
    KeyUnknown, //                   0x9B ???
    KeyUnknown, //                   0x9C ???
    KeyUnknown, //                   0x9D ???
    KeyUnknown, //                   0x9E ???
    KeyUnknown, //                   0x9F ???
    KeyLeftShift,   // VK_LSHIFT         0xA0
    KeyRightShift,  // VK_RSHIFT         0xA1
    KeyLeftCtrl,    // VK_LCONTROL       0xA2
    KeyRightCtrl,   // VK_RCONTROL       0xA3
    KeyLeftAlt,     // VK_LMENU          0xA4
    KeyRightAlt,    // VK_RMENU          0xA5
    KeyUnknown, // VK_BROWSER_BACK        0xA6
    KeyUnknown, // VK_BROWSER_FORWARD     0xA7
    KeyUnknown, // VK_BROWSER_REFRESH     0xA8
    KeyUnknown, // VK_BROWSER_STOP        0xA9
    KeyUnknown, // VK_BROWSER_SEARCH      0xAA
    KeyUnknown, // VK_BROWSER_FAVORITES   0xAB
    KeyUnknown, // VK_BROWSER_HOME        0xAC
    KeyUnknown, // VK_VOLUME_MUTE         0xAD
    KeyUnknown, // VK_VOLUME_DOWN         0xAE
    KeyUnknown, // VK_VOLUME_UP           0xAF
    KeyUnknown, // VK_MEDIA_NEXT_TRACK    0xB0
    KeyUnknown, // VK_MEDIA_PREV_TRACK    0xB1
    KeyUnknown, // VK_MEDIA_STOP          0xB2
    KeyUnknown, // VK_MEDIA_PLAY_PAUSE    0xB3
    KeyUnknown, // VK_LAUNCH_MAIL         0xB4
    KeyUnknown, // VK_LAUNCH_MEDIA_SELECT 0xB5
    KeyUnknown, // VK_LAUNCH_APP1         0xB6
    KeyUnknown, // VK_LAUNCH_APP2         0xB7
    KeyUnknown, //                   0xB8 ???
    KeyUnknown, //                   0xB9 ???
    KeyUnknown, // VK_OEM_1          0xBA   // ';:' for US
    KeyUnknown, // VK_OEM_PLUS       0xBB
    KeyUnknown, // VK_OEM_COMMA      0xBC
    KeyMinus,   // VK_OEM_MINUS      0xBD
    KeyDot,     // VK_OEM_PERIOD     0xBE
    KeySlash,   // VK_OEM_2          0xBF
    KeyUnknown, // VK_OEM_3          0xC0   // '`~' for US
    KeyUnknown, //                   0xC1 ???
    KeyUnknown, //                   0xC2 ???
    KeyUnknown, //                   0xC3 ???
    KeyUnknown, //                   0xC4 ???
    KeyUnknown, //                   0xC5 ???
    KeyUnknown, //                   0xC6 ???
    KeyUnknown, //                   0xC7 ???
    KeyUnknown, //                   0xC8 ???
    KeyUnknown, //                   0xC9 ???
    KeyUnknown, //                   0xCA ???
    KeyUnknown, //                   0xCB ???
    KeyUnknown, //                   0xCC ???
    KeyUnknown, //                   0xCD ???
    KeyUnknown, //                   0xCE ???
    KeyUnknown, //                   0xCF ???
    KeyUnknown, //                   0xD0 ???
    KeyUnknown, //                   0xD1 ???
    KeyUnknown, //                   0xD2 ???
    KeyUnknown, //                   0xD3 ???
    KeyUnknown, //                   0xD4 ???
    KeyUnknown, //                   0xD5 ???
    KeyUnknown, //                   0xD6 ???
    KeyUnknown, //                   0xD7 ???
    KeyUnknown, //                   0xD8 ???
    KeyUnknown, //                   0xD9 ???
    KeyUnknown, //                   0xDA ???
    KeyUnknown, // VK_OEM_4          0xDB  //  '[{' for US
    KeyUnknown, // VK_OEM_5          0xDC  //  '\|' for US
    KeyUnknown, // VK_OEM_6          0xDD  //  ']}' for US
    KeyUnknown, // VK_OEM_7          0xDE  //  ''"' for US
    KeyUnknown, // VK_OEM_8          0xDF
    KeyUnknown, //                   0xE0 ???
    KeyUnknown, // VK_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
    KeyUnknown, // VK_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
    KeyUnknown, // VK_ICO_HELP       0xE3  //  Help key on ICO
    KeyUnknown, // VK_ICO_00         0xE4  //  00 key on ICO
    KeyUnknown, // VK_PROCESSKEY     0xE5
    KeyUnknown, // VK_ICO_CLEAR      0xE6
    KeyUnknown, // VK_PACKET         0xE7
    KeyUnknown, //                   0xE8 ???
    KeyUnknown, // VK_OEM_RESET      0xE9
    KeyUnknown, // VK_OEM_JUMP       0xEA
    KeyUnknown, // VK_OEM_PA1        0xEB
    KeyUnknown, // VK_OEM_PA2        0xEC
    KeyUnknown, // VK_OEM_PA3        0xED
    KeyUnknown, // VK_OEM_WSCTRL     0xEE
    KeyUnknown, // VK_OEM_CUSEL      0xEF
    KeyUnknown, // VK_OEM_ATTN       0xF0
    KeyUnknown, // VK_OEM_FINISH     0xF1
    KeyUnknown, // VK_OEM_COPY       0xF2
    KeyUnknown, // VK_OEM_AUTO       0xF3
    KeyUnknown, // VK_OEM_ENLW       0xF4
    KeyUnknown, // VK_OEM_BACKTAB    0xF5
    KeyUnknown, // VK_ATTN           0xF6
    KeyUnknown, // VK_CRSEL          0xF7
    KeyUnknown, // VK_EXSEL          0xF8
    KeyUnknown, // VK_EREOF          0xF9
    KeyUnknown, // VK_PLAY           0xFA
    KeyUnknown, // VK_ZOOM           0xFB
    KeyUnknown, // VK_NONAME         0xFC
    KeyUnknown, // VK_PA1            0xFD
    KeyUnknown  // VK_OEM_CLEAR      0xFE
  };

  bool WindowsKeyState::processEvent(UINT msg, WPARAM wParam, LPARAM lParam)
  {
    bool down = false;
    switch (msg)
    {
    case WM_KEYDOWN:
      if (lParam & (1<<30)) // ignore repeat
        break;
      down = true;
    case WM_KEYUP:
        if (wParam < 256)
          return key(winkey_to_kapusha_[wParam], down, GetTickCount());
    }
    return false;
  }

///////////////////////////////////////////////////////////////////////////////

  class WindowsPointerState : public PointerState
  {
  public:
    WindowsPointerState()
      : grabbed_(false), ignore_(false) {}
    bool processEvent(UINT msg, WPARAM wParam, LPARAM lParam);
    
    void resize(vec2i size) {
      size2_ = size / 2;
      scale_ = vec2f(1.f, -1.f) / vec2f(size);
    }

    void setGrab(bool _grab, HWND window)
    {
      grabbed_ = _grab;
      window_ = window;
      if (grabbed_) grab();
    }
    
  private:
    void grab()
    {
      POINT pt;
      pt.x = size2_.x;
      pt.y = size2_.y;
      ClientToScreen(window_, &pt);
      SetCursorPos(pt.x, pt.y);
      ignore_ = true;
    }
    vec2f pos(LPARAM lParam) {
      return vec2f(static_cast<float>(GET_X_LPARAM(lParam)),
                   static_cast<float>(GET_Y_LPARAM(lParam))) * scale_ * 2.f
             + vec2f(-1.f, 1.f);
    }

    vec2f delta(LPARAM lParam) {
      return vec2f(static_cast<float>(GET_X_LPARAM(lParam) - size2_.x),
                   static_cast<float>(GET_Y_LPARAM(lParam) - size2_.y)) * scale_;
    }

    HWND window_;
    vec2i size2_;
    vec2f scale_;
    bool grabbed_, ignore_;
  };

  bool WindowsPointerState::processEvent(UINT msg, WPARAM wParam, LPARAM lParam)
  {
    int now = GetTickCount();
    switch(msg)
    {
    case WM_LBUTTONDOWN:
      mouseClick(pos(lParam), Pointer::LeftButton, now);
      break;

    case WM_LBUTTONUP:
      mouseUnclick(pos(lParam), Pointer::LeftButton, now);
      break;

    case WM_MOUSEMOVE:
      if (ignore_) { ignore_ = false; break; }

      if (grabbed_)
      {
        mouseMove(pos(lParam), delta(lParam), now);
        grab();
      } else
        mouseMove(pos(lParam), now);
      break;

    default:
      return false;
    }
    return true;
  }

///////////////////////////////////////////////////////////////////////////////
  class WindowController : public IViewportController
  {
  public:
    WindowController(HINSTANCE hInst, IViewport *viewport, int width, int height, bool fullscreen);
    ~WindowController() {}

    virtual void quit(int code);
    virtual void requestRedraw();
    virtual void limitlessPointer(bool limitless);
    virtual void hideCursor(bool hide);
    virtual const PointerState& pointerState() const { return pointers_; }
    virtual const KeyState& keyState() const { return keys_; }

    int run();

  protected:
    friend LRESULT CALLBACK windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT processEvent(UINT msg, WPARAM wParam, LPARAM lParam);

  private:
    IViewport *viewport_;
    HWND window_;
    HDC dc_;
    vec2f scale_;
    bool need_redraw_;
    WindowsPointerState pointers_;
    WindowsKeyState keys_;
  };

  void WindowController::quit(int code)
  {
    PostQuitMessage(code);
  }

  void WindowController::requestRedraw()
  {
    need_redraw_ = true;
  }

  void WindowController::limitlessPointer(bool limitless)
  {
    pointers_.setGrab(limitless, window_);
  }

  void WindowController::hideCursor(bool hide)
  {
    ShowCursor(!hide);
  }

///////////////////////////////////////////////////////////////////////////////

  LRESULT WindowController::processEvent(UINT msg, WPARAM wParam, LPARAM lParam)
  {
    switch (msg)
    {
    case WM_SIZE:
      {
        vec2i size(lParam & 0xffff, lParam >> 16);
        pointers_.resize(size);
        scale_ = vec2f(1.f) / vec2f(size);
        viewport_->resize(size);
      }
      break;

    case WM_KEYDOWN:
    case WM_KEYUP:
      if (keys_.processEvent(msg, wParam, lParam))
        viewport_->inputKey(keys_);
      break;

    case WM_CLOSE:
    case WM_DESTROY:
  	  PostQuitMessage(0);
      break;

    case WM_QUIT: //! \fixme we don't get here
      viewport_->close();
      break;

    default:
      if (pointers_.processEvent(msg, wParam, lParam))
      {
        viewport_->inputPointer(pointers_);
        break;
      } else
        return DefWindowProc(window_, msg, wParam, lParam);
	  }
    return 0;
  }

  LRESULT CALLBACK windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    WindowController *winctl = reinterpret_cast<WindowController*>(
      GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (winctl)
      return winctl->processEvent(msg, wParam, lParam);

    if (msg == WM_NCCREATE)
    {
      LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>(lParam);
      SetWindowLongPtr(hWnd, GWLP_USERDATA,
                       reinterpret_cast<LONG_PTR>(create_struct->lpCreateParams));
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
  }

///////////////////////////////////////////////////////////////////////////////

  WindowController::WindowController(HINSTANCE hInst, IViewport *viewport,
                                     int width, int height, bool fullscreen)
    : viewport_(viewport)
    , need_redraw_(true)
  {
    WNDCLASSEX wndclass;
    ZeroMemory(&wndclass, sizeof wndclass);
    wndclass.cbSize = sizeof wndclass;
    wndclass.style = CS_OWNDC;
    wndclass.lpfnWndProc = windowProc;
    wndclass.hInstance = hInst;
    wndclass.lpszClassName = L"KapushaWindowClass";
    RegisterClassEx(&wndclass);

    window_ = CreateWindow(wndclass.lpszClassName, 0,
                           WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                           0, 0, width, height,
                           0, 0, hInst, this);

    dc_ = GetDC(window_);
    static const PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR), 1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
      32, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 32, 0, 0,
      PFD_MAIN_PLANE, 0, 0, 0, 0 };
    SetPixelFormat(dc_, ChoosePixelFormat(dc_, &pfd), &pfd);
    wglMakeCurrent(dc_, wglCreateContext(dc_));

    glewInit();

    viewport->init(this);
  }

  int WindowController::run()
  {
    MSG message;
    int prev = GetTickCount(), now;
    while (true)
    {
      if (need_redraw_)
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
	      {
			    TranslateMessage(&message);
			    DispatchMessage(&message);

          if (message.message == WM_QUIT)
            return (int) message.wParam;
        }
      else 
        if (!GetMessage(&message, NULL, 0, 0))
          break;

      //if (need_redraw_)
      {
        //need_redraw_ = false;
        now = GetTickCount();
        viewport_->draw(now, (now - prev) / 1000.f);
        SwapBuffers(dc_);
        prev = now;
      }
	  }
	  return (int) message.wParam;
  }

///////////////////////////////////////////////////////////////////////////////

  int RunWindow(HINSTANCE hInst, IViewport *viewport,
                int width, int height, bool fullscreen)
  {
    WindowController winctl(hInst, viewport, width, height, fullscreen);
    return winctl.run();
  }

} // namespace kapusha
