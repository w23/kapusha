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
    virtual IViewport *create(IViewportController *controller) const = 0;
  protected:
    //! This isn't meant to be deleted inside kapusha
    virtual ~IViewportFactory() {}
  };
} // namespace kapusha
