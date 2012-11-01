#pragma once

namespace kapusha {

  class ISystem {
  public:
    virtual void quit(int code = 0) = 0;
    virtual void redraw() = 0;
  };
  
  class IViewport {
  public:
    virtual ~IViewport() {}
    virtual void init(ISystem* system) = 0;
    virtual void resize(int width, int height) = 0;
    virtual void draw(int ms) = 0;
  
    /*! \todo
  public:
     struct PointerEvent {
     #define KAPUSHA_MAX_POINTERS 16
     };

    virtual void eventKey() = 0;
    virtual void eventPointer() = 0;
     */
  };
  
} // namespace kapusha