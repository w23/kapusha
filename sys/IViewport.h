#pragma once

#include "../math/types.h"

namespace kapusha {
  
#define MAX_POINTERS_IN_EVENT 8

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
  
  public:
     struct PointerEvent {
       struct Pointer {
         enum {
           None = 0,
           Pressed = 1,
           Move = 2,
           LeftButton = 4,
           MiddleButton = 8,
           WheelUp = 16,
           WheelDown = 32
         } flags;
         math::vec2f point;
         float pressure;
       };
       
       int combined_flags_;
       Pointer pointers_[MAX_POINTERS_IN_EVENT];
     };

    virtual void pointerEvent(const PointerEvent& event) = 0;
  };
  
} // namespace kapusha