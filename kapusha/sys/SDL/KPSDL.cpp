#include <SDL/SDL.h>
#include <kapusha/core/IViewport.h>

#if KAPUSHA_RPI
#include "RPi.h"
#endif

namespace kapusha {

///////////////////////////////////////////////////////////////////////////////

  class SDLKeyState : public KeyState
  {
  public:
    bool processEvent(const SDL_Event& e);
  };

  bool SDLKeyState::processEvent(const SDL_Event& e)
  {
    int keycode = KeyUnknown;
    bool pressed = false;

    switch (e.type)
    {
    case SDL_KEYDOWN:
      pressed = true;
    case SDL_KEYUP:
      //! \todo mapping array?
      if (e.key.keysym.sym < 64)
        keycode = e.key.keysym.sym;
      else if (e.key.keysym.sym > 96 && e.key.keysym.sym < 123)
        keycode = e.key.keysym.sym + KeyA - SDLK_a;
      else switch (e.key.keysym.sym) {
        case SDLK_DELETE: keycode = KeyDel; break;
        case SDLK_UP: keycode = KeyUp; break;
        case SDLK_DOWN: keycode = KeyDown; break;
        case SDLK_LEFT: keycode = KeyLeft; break;
        case SDLK_RIGHT: keycode = KeyRight; break;
        case SDLK_LSHIFT: keycode = KeyShift; break;
        //! \todo more
        default: return false;
      }
      break;

    default:
      return false;
    }

    return key(keycode, pressed, SDL_GetTicks());
  }

///////////////////////////////////////////////////////////////////////////////

  class SDLPointerState : public PointerState
  {
  public:
    SDLPointerState() : delta_only_(false) {}
    void resize(vec2i size) {
      scale_ = vec2f(1.f) / vec2f(size.x, -size.y);
    }
    void setDeltaOnly(bool delta) {
      delta_only_ = delta;
    }
    bool processEvent(const SDL_Event& e);
  private:
    inline vec2f transform(int x, int y) {
      return vec2f(x, y) * scale_ * 2.f + vec2f(-1.f, 1.f);
    }

    vec2f scale_;
    bool delta_only_;
  };

  bool SDLPointerState::processEvent(const SDL_Event& e)
  {
    int now = SDL_GetTicks();
    switch (e.type)
    {
      case SDL_MOUSEMOTION:
        if (delta_only_)
          mouseMove(transform(e.motion.x, e.motion.y),
                    vec2f(e.motion.xrel, e.motion.yrel) * scale_, now);
        else
          mouseMove(transform(e.motion.x, e.motion.y), now);
        break;

      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
      {
        int button;
        vec2f pos = transform(e.button.x, e.button.y);

        switch(e.button.button)
        {
          case SDL_BUTTON_LEFT:
            button = Pointer::LeftButton;
            break;
          case SDL_BUTTON_RIGHT:
            button = Pointer::RightButton;
            break;
          case SDL_BUTTON_MIDDLE:
            button = Pointer::MiddleButton;
            break;
          default: //! \fixme wheel support
            return false;
        }

        if (e.button.state == SDL_PRESSED)
          mouseClick(pos, button, now);
        else
          mouseUnclick(pos, button, now);
      }
        break;

      default:
        return false;
    }
    return true;
  }
 

///////////////////////////////////////////////////////////////////////////////

  class SDLViewportController : public IViewportController
  {
  public:
    SDLViewportController(IViewport *viewport, vec2i size);
    ~SDLViewportController();
    bool processEvents();

  public:
    virtual void quit(int code) {
      should_loop_ = false;
    }
    virtual void requestRedraw() {
      //! \fixme
    }
    virtual void limitlessPointer(bool limitless) {
      SDL_WM_GrabInput(limitless ? SDL_GRAB_ON : SDL_GRAB_OFF);
      pointer_state_.setDeltaOnly(limitless);
    }
    virtual void hideCursor(bool hide) {
      SDL_ShowCursor(hide ? SDL_DISABLE : SDL_ENABLE);
    }
    virtual const PointerState& pointerState() const {
      return pointer_state_;
    }
    virtual const KeyState& keyState() const {
      return key_state_;
    }

  private:
    IViewport *viewport_;
    bool should_loop_;
    SDLKeyState key_state_;
    SDLPointerState pointer_state_;
  };

  SDLViewportController::SDLViewportController(IViewport *viewport, vec2i size)
    : viewport_(viewport)
    , should_loop_(true)
  {
    viewport_->init(this);
    viewport_->resize(size);
    pointer_state_.resize(size);
  }

  SDLViewportController::~SDLViewportController()
  {
    viewport_->close();
  }

  bool SDLViewportController::processEvents()
  {
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
      switch (e.type)
      {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
          if (key_state_.processEvent(e))
            viewport_->inputKey(key_state_);
          break;

        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          if (pointer_state_.processEvent(e))
            viewport_->inputPointer(pointer_state_);
          break;

        case SDL_VIDEORESIZE:
          {
            vec2i size(e.resize.w, e.resize.h);
            viewport_->resize(size);
            pointer_state_.resize(size);
          }
          break;

        case SDL_QUIT:
          should_loop_ = false;
      }
    }

    return should_loop_;
  } // SDLViewportController::processEvents()

///////////////////////////////////////////////////////////////////////////////

  int KPSDL(IViewport* viewport, int width, int height, bool fullscreen)
  {
    //! \fixme error check
    SDL_Init(SDL_INIT_VIDEO);

    u32 flags = SDL_DOUBLEBUF | (fullscreen ? SDL_FULLSCREEN : 0);
      //! \todo | SDL_RESIZABLE

#if KAPUSHA_RPI
    RPiGLES rpigles;
    KP_ASSERT(rpigles.initGL(width, height));
    width = rpigles.width();
    height = rpigles.height();
#else // !KAPUSHA_RPI

    flags |= SDL_OPENGL;

    if (width == -1 || height == -1)
    {
      const SDL_VideoInfo* vinfo = SDL_GetVideoInfo();
      width = vinfo->current_w;
      height = vinfo->current_h;
    }

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#endif // !KAPUSHA_RPI

    SDL_SetVideoMode(width, height, 32, flags);

    {
      SDLViewportController viewctl(viewport, vec2i(width, height));

      unsigned prev = SDL_GetTicks();
      while (viewctl.processEvents())
      {
        unsigned now = SDL_GetTicks();
        float dt = (now - prev) / 1000.f;
        viewport->draw(now, dt);
        prev = now;
#if KAPUSHA_RPI
        rpigles.swap();
#else
        SDL_GL_SwapBuffers();
#endif
      }
    }

#if KAPUSHA_RPI
    rpigles.closeGL();
#endif

    SDL_Quit();
    return 0;
  } // KPSDL

} // namespace kapusha
