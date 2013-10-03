#pragma once
#include "input.h"

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
} // namespace kapusha
