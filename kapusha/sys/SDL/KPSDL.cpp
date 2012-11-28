#include <SDL/SDL.h>
#include <kapusha/core/IViewport.h>

namespace kapusha {

///////////////////////////////////////////////////////////////////////////////

  class SDLKeyState : public KeyState
  {
  public:
    void processEvent(const SDL_Event& e);
  };

  void SDLKeyState::processEvent(const SDL_Event& e)
  {
    //! \fixme
  }

///////////////////////////////////////////////////////////////////////////////

  class SDLPointerState : public PointerState
  {
  public:
    SDLPointerState() : delta_only_(false) {}
    void resize(vec2i size) {
      scale_ = vec2f(1.f) / vec2f(size);
    }
    void setDeltaOnly(bool delta) {
      delta_only_ = delta;
    }
    void processEvent(const SDL_Event& e);
  private:
    vec2f scale_;
    bool delta_only_;
  };

  void SDLPointerState::processEvent(const SDL_Event& e)
  {
    //! \fixme
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
          key_state_.processEvent(e);
          viewport_->inputKey(key_state_);
          break;

        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          pointer_state_.processEvent(e);
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

    if (width == -1 || height == -1)
    {
      const SDL_VideoInfo* vinfo = SDL_GetVideoInfo();
      width = vinfo->current_w;
      height = vinfo->current_h;
    }

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_SetVideoMode(width, height, 32, SDL_DOUBLEBUF | SDL_OPENGL
      //! \todo | SDL_RESIZABLE
                     | (fullscreen ? SDL_FULLSCREEN : 0));

    {
      SDLViewportController viewctl(viewport, vec2i(width, height));

      unsigned prev = SDL_GetTicks();
      while (viewctl.processEvents())
      {
        unsigned now = SDL_GetTicks();
        float dt = (now - prev) / 1000.f;
        viewport->draw(now, dt);
        prev = now;
        SDL_GL_SwapBuffers();
      }
    }

    SDL_Quit();
    return 0;
  } // KPSDL

} // namespace kapusha
