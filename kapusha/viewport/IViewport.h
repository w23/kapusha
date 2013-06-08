#pragma once
#include "input.h"

namespace kapusha {
  class Context;
  //! An interface for doing OS/platform-specific stuff
  class IViewportController {
  public:
    //! Quit the application with return code
    //! \param code Return code
    virtual void quit(int code = 0) = 0;
    
    //! Request redraw
    //! \warning It is safe to call this from any thread
    virtual void requestRedraw() = 0;
    
    //! Request fullscreen mode
    //! Requests fullscreen mode with native resolution
    //! \todo virtual void requestFullscreen() = 0;

    //! Set the mouse cursor free of any window/screen boundaries
    //! \param relative_only Set this true if you want mouse-aiming
    //! \warning Pointer::getPosition() will return garbage if this is set to true
    virtual void setRelativeOnlyPointer(bool relative_only) = 0;
    
    //! Hide mouse cursor
    //! \param hide Does cursor need to be hidden
    virtual void hideCursor(bool hide) = 0;
    
    //! Current pointer state
    //! \return current pointer input state
    virtual const PointerState& pointerState() const = 0;
    
    //! Current key state
    //! \return current key input state
    virtual const KeyState& keyState() const = 0;
  };
  
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
