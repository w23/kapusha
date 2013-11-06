#pragma once
#include "Pointers.h"
#include "Keys.h"

namespace kapusha {
  //! An interface for doing OS/platform-specific stuff
  class IViewportController {
  public:
    //! Quit the application with return code
    //! \param code Return code
    virtual void quit(int code = 0) = 0;
    
    //! Request redraw
    //! \warning It is safe to call this from any thread
    //! \todo virtual void request_redraw() = 0;
    
    //! Request fullscreen mode
    //! Requests fullscreen mode with native resolution
    //! \todo virtual void request_fullscreen() = 0;
    
    //! Grab user input
    //! This means:
    //!  * pointers are grabbed
    //!  * pointers return delta only data, their positions bear no meaning
    //!  * mouse cursor is hidden
    //! \param grab True means grab, false means release
    virtual void grab_input(bool grab) = 0;
    
    //! Current pointers state
    //! \return current pointer input state
    virtual const Pointers& pointers() const = 0;
    
    //! Current keys state
    //! \return current key input state
    virtual const Keys& keys() const = 0;
  };
} // namespace kapusha
