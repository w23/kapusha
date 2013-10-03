#pragma once
#include "input.h"
#include "IViewportController.h"

namespace kapusha {
  //! Viewport is the place where all the fun happens
  //! Lifetime <= rendering context lifetime
  class IViewport {
  public:
    //! Called just before the current gl context will become invalid.
    virtual ~IViewport() {}
    
    //! Viewport window size has been determined or changed
    //! Called for the first time just after init()
    //! \param size New size in pixels
    virtual void resize(vec2i size) = 0;
    
    //! User input
    virtual void inputPointer(const PointerState& pointers) {}
    virtual void inputKey(const KeyState& keys) {}
    
    //! Paint stuff
    //! \param ms Monotonic time in milliseconds since some referential point
    //! \param dt Time since previous draw(), in seconds
    virtual void draw(int ms, float dt) = 0;
  };
  
  
  class IViewportFactory {
  public:
    struct Preferences {
      const char *window_title;
      u32 flags;
      vec2i prefer_resolution;
      int prefer_framerate;
      u8 depth_bits;
      u8 stencil_bits;

      enum Flags {
        FlagFullscreen = 0x0001,
        //FlagOculus = 0x0002,
        //FlagRequireOculus = 0x01000002
        FlagOpenGL2_1 = 0x0100,
        FlagOpenGLES2 = 0x0200,
        //FlagOpenGL3_2 = 0x0400,
        //FlagOpenGL4_0 = 0x0800,
        //OpenGLES3 = 0x1000,
      };

      Preferences()
        : window_title("kapusha")
        , flags(0)
        , prefer_resolution(1280, 720)
        , prefer_framerate(60)
        , depth_bits(24)
        , stencil_bits(8)
      {}
    };
  public:
    virtual IViewport *create(IViewportController *controller) const = 0;
    virtual const Preferences &preferences() const = 0;
  protected:
    //! This isn't meant to be deleted inside kapusha
    virtual ~IViewportFactory() {}
  };
} // namespace kapusha
