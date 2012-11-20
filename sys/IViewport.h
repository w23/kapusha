#pragma once

#include "../math/types.h"

namespace kapusha {
  
#define MAX_POINTERS_IN_EVENT 8

  //! An interface for doing OS/platform-specific stuff
  class ISystem {
  public:
    //! Quit the application with return code
    //! \param code Return code
    virtual void quit(int code = 0) = 0;
    
    //! Request system to IViewport::draw() when appropriate
    //! Call this every IViewport::draw() call to make constant animation
    virtual void redraw() = 0;

    //! Reset pointer position to center
    //! Useful for mouse-aiming
    virtual void pointerReset() = 0;
  };
  
  //! Viewport is a place where all the fun happens
  class IViewport {
  public: // Basic stuff
    virtual ~IViewport() {}
    
    //! Called when GL context is up and set, just before the mainloop
    //! \param system Pointer to ISystem interface, while you don't own it,
    //! it will be valid for the whole lifetime of this viewport.
    virtual void init(ISystem* system) = 0;
    
    //! Viewport window size has changed
    //! it is also called for the first time just after init()
    //! \param width new width in pixels
    //! \param height new height in pixels
    virtual void resize(int width, int height) = 0;
    
    //! Paint stuff
    //! \param ms Monotonic time in milliseconds since some referential point
    //! \param dt Time since previous draw(), in seconds
    virtual void draw(int ms, float dt) = 0;
    
    //! Deinitialize
    //! Called just before the current gl context will become invalid.
    //! \warning this is not equal to dtor, as init() could be called again
    //! for a new context
    virtual void close() = 0;
  
  public: // User input
    //! Some user-input Event
    class Event {
    public:
      Event(int type, int time = 0) : type_(type), time_(time) {}
      
      //! Get type of the event
      int type() const { return type_; }
      
      //! Get time in milliseconds when this event was registered by system.
      //! \warning May be inaccurate on some systems, i.e. quantized by framerate.
      int time() const { return time_; }
      
      enum Type {
        Key,
        TextInput,
        Pointer,
        //! \todo Orientation
        Custom
      };
      
    private:
      int type_;
      int time_;
    };
    
    //! Some user event happened
    //! \param event The user-input event
    //! default implementation switches events by type and calls relevant methods
    virtual void userEvent(const Event& event);
    
    //! Event of keys
    class KeyEvent : public Event {
    public:
      bool isPressed() const { return pressed_; }
      int key() const { return key_; }
      enum Keys {
        KeyBackspace = 8,
        KeyEnter = 13,
        KeyEsc = 27,
        KeyPageUp = 33,
        KeyPageDown = 34,
        KeyLeft = 37,
        KeyUp = 38,
        KeyRight = 39,
        KeyDown = 40
        //! \todo more
      };
      
      //! \todo modifiers
      int modifiers() const { return modifiers_; }
      enum Modifiers {
        ModLShift,
        ModLCtrl,
        ModLAlt
        //! \todo apple?
      };
      
    public:
      KeyEvent(int key, int modifiers, bool press = true, int time = 0)
        : Event(Key, time)
        , pressed_(press), key_(key), modifiers_(modifiers) {}
      
    private:
      bool pressed_;
      int key_;
      int modifiers_;
    };
    virtual void keyEvent(const KeyEvent& event) {}
    
    //! \todo
    class TextInputEvent : public Event {
    };
    virtual void textInputEvent(const TextInputEvent& event) {}
    
    //! Event of pointers -- mouse, touches and such
    class PointerEvent : public Event {
    public:
      struct Pointer {
        enum {
          None = 0,
          Move = 1,
          Pressed = 2,
          //! mouse-specific
          LeftButton = Pressed,
          RightButton = 4,
          MiddleButton = 8,
          WheelUp = 16,
          WheelDown = 32
        };
        int flags;
        math::vec2f point;
         
        //! \todo
        float pressure;
         
        Pointer(math::vec2f _pos = math::vec2f(0), int _flags = None,
                float _pressure = 1.f)
        : flags(_flags), point(_pos), pressure(_pressure) {}
      };
      
      PointerEvent()
      : Event(Event::Pointer, 0)
      , combined_flags_(0) {}
      
      //! mouse event
      PointerEvent(math::vec2f pos, int flags)
      : Event(Event::Pointer, 0)
      , combined_flags_(flags)
      {
        pointers_[0] = Pointer(pos, flags);
      }
      
      const Pointer& main() const { return pointers_[0]; }

    protected:
       int combined_flags_;
       Pointer pointers_[MAX_POINTERS_IN_EVENT];
     };
    virtual void pointerEvent(const PointerEvent& event) {}
  };
  
} // namespace kapusha
