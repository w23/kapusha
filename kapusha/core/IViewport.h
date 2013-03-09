#pragma once
#include "Input.h"

namespace kapusha {
  //! An interface for doing OS/platform-specific stuff
  class IViewportController {
  public:
    //! Quit the application with return code
    //! \param code Return code
    virtual void quit(int code = 0) = 0;
    
    //! Request redraw
    //! \warning It is safe to call this from any thread
    virtual void requestRedraw() = 0;

    //! Set the mouse cursor free of any window/screen boundaries
    /*! \warning actually this pins the mouse cursors at the viewport center,
     * therefore stripping any sense from Pointer::point field.
     * Use Pointer::movement instead
     */
    //! \param limitless Set this true if you want mouse-aiming
    virtual void limitlessPointer(bool limitless) = 0;
    
    //! Hide mouse cursor
    //! \param hide does cursor need to be hidden
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
  public: // Basic stuff
    virtual ~IViewport() {}
    
    //! Called when GL context is up and set, just before the mainloop
    //! \param system Pointer to ISystem interface, while you don't own it,
    //! it will be valid for the whole lifetime of this viewport.
    virtual void init(IViewportController* controller) = 0;
    
    //! Viewport window size has changed
    //! it is also called for the first time just after init()
    //! \param width new width in pixels
    //! \param height new height in pixels
    virtual void resize(vec2i) = 0;
    
    //! User input
    virtual void inputPointer(const PointerState& pointers) {}
    virtual void inputKey(const KeyState& keys) {}
    
    //! Paint stuff
    //! \param ms Monotonic time in milliseconds since some referential point
    //! \param dt Time since previous draw(), in seconds
    virtual void draw(int ms, float dt) = 0;
    
    //! Deinitialize
    //! Called just before the current gl context will become invalid.
    //! \warning this is not equal to dtor, as init() could be called again
    //! for a new context
    virtual void close() = 0;
  };
  
} // namespace kapusha
