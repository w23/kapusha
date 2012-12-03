#include <windows.h>
#include <kapusha/core/Log.h>
#include <kapusha/core/IViewport.h>
#include <kapusha/render/OpenGL.h>

///////////////////////////////////////////////////////////////////////////////
namespace kapusha {
  class WindowController : public IViewportController
  {
  public:
    WindowController(HINSTANCE hInst, IViewport *viewport, int width, int height, bool fullscreen);
    ~WindowController() {}

    virtual void quit(int code);
    virtual void requestRedraw();
    virtual void limitlessPointer(bool limitless) {}
    virtual void hideCursor(bool hide) {}
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
    PointerState pointers_;
    KeyState keys_;
  };

  void WindowController::quit(int code)
  {
    PostQuitMessage(code);
  }

  void WindowController::requestRedraw()
  {
    need_redraw_ = true;
  }

  /*
  virtual void pointerReset()
  {
    POINT pt;
    pt.x = w / 2;
    pt.y = h / 2;
    ClientToScreen(hWnd, &pt);
    SetCursorPos(pt.x, pt.y);
    ignore_move_ = true;
  }
  */

///////////////////////////////////////////////////////////////////////////////
  /*
  int keyTranslate(int key)
  {
    if (key > 0x29 && key < 0x40)
      return key;

    if (key > 0x40 && key < 0x5B)
        return key + 0x20;

    switch (key)
    {
    case VK_UP:
      return IViewport::KeyEvent::KeyUp;
    case VK_DOWN:
      return IViewport::KeyEvent::KeyDown;
    case VK_LEFT:
      return IViewport::KeyEvent::KeyLeft;
    case VK_RIGHT:
      return IViewport::KeyEvent::KeyRight;
    default:
      L("Uknown key code %d", key);
      return key;
    }
  }
  */

  LRESULT WindowController::processEvent(UINT msg, WPARAM wParam, LPARAM lParam)
  {
    switch (msg)
    {
    case WM_SIZE:
      {
        scale_ = vec2f(1.f) / vec2f(static_cast<float>(lParam & 0xffff),
                                    static_cast<float>(lParam >> 16));
        viewport_->resize(vec2i(lParam & 0xffff, lParam >> 16));
      }
      break;

/*
    case WM_KEYDOWN:
      if (!(lParam & (1<<30))) // ignore repeat
        viewport_->userEvent(IViewport::KeyEvent(keyTranslate(wParam), 0));
      break;
  
    case WM_KEYUP:
      viewport_->userEvent(IViewport::KeyEvent(keyTranslate(wParam), 0, false));
      break;

    case WM_MOUSEMOVE:
      {
        if (system_win.ignore_move_) { system_win.ignore_move_ = false; break; }
        viewport_->userEvent(IViewport::PointerEvent(math::vec2f(lParam&0xffff, lParam >> 16),
                                                    IViewport::PointerEvent::Pointer::Move));
      }
      break;
*/

    case WM_CLOSE:
    case WM_DESTROY:
  	  PostQuitMessage(0);
      break;

    case WM_QUIT: //! \fixme we don't get here
      viewport_->close();
      break;

    default:
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
