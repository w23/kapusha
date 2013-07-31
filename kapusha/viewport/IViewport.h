#pragma once
#include "input.h"
#include "IViewportController.h"

namespace kapusha {
  class Context;
  
  //! Viewport is a place where all the fun happens
  class IViewport {
  public:
    virtual ~IViewport() {}
    
    //! Called when GL context is up and set current, just before the mainloop
    //! \param controller Pointer to IViewportController interface that will be
    //!                   valid until close()
    //! \param context The context
    //! it will be valid for the whole lifetime of this viewport.
    virtual void init(IViewportController* controller, Context *context) = 0;
    
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
    
    //! Deinitialize
    //! Called just before the current gl context will become invalid.
    //! \warning this is not equal to dtor, as init() can be called again
    //! for a new context
    virtual void close() = 0;
  };
} // namespace kapusha
