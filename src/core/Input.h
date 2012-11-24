#pragma once
#include "Core.h"
#include "../math/types.h"

#if KAPUSHA_MULTITOUCH
#define MAX_POINTERS_IN_EVENT 8
#else
#define MAX_POINTERS_IN_EVENT 1
#endif

namespace kapusha {
  
  //! Basic input state with timestamp
  class InputState {
  public:
    inline void update(u32 time) {
      delta_ = time - last_changed_;
      last_changed_ = time;
    }
    inline u32 timestamp() const { return last_changed_; }
    inline u32 delta() const { return  delta_; }
    
  private:
    u32 last_changed_;
    u32 delta_;
  };

////////////////////////////////////////////////////////////////////////////////
  //! State of a input by pointers
  class PointerState : public InputState
  {
  public:
    //! Single pointer
    struct Pointer {
      enum {
        //! pointer is inactive
        None = 0,
        
        //! pointer has moved
        Move = 1, //! event flag, cleared
        
        //! touch down or left mouse button
        Pressed = 2,
        //! mouse-specific
        LeftButton = Pressed,
        RightButton = 4,
        MiddleButton = 8,
      };
      //! current state & events
      int flags;
      
      //! current position
      vec2f point;
      
      //! change of position
      vec2f movement;
      
      //! \todo
      //vec2f speed;
      //float pressure;
      
      Pointer(vec2f _pos = vec2f(0), int _flags = None)
      : flags(_flags), point(_pos) {}
      
      void update(vec2f position, int _flags, int flags_remove = 0)
      {
        movement = position - point;
        point = position;
        flags = (flags | _flags) ^ flags_remove;
      }
      
    protected:
      friend class PointerState;
      void clearEventFlags() { flags ^= Move; }
      void clearAllFlags() { flags = 0; }
    };
    
  public:
    PointerState() : combined_flags_(0), changed_flags_(0) {}
    const Pointer& main() const { return pointers_[0]; }
    
    void mouseMove(vec2f to, u32 time = 0);
    void mouseClick(vec2f at, int button, u32 time = 0);
    void mouseUnclick(vec2f at, int button, u32 time = 0);
    
  private:
    void mouseUpdate(vec2f at, int flags_add, int flags_remove, u32 time);
    
  protected:
    int combined_flags_;
    int changed_flags_;
    Pointer pointers_[MAX_POINTERS_IN_EVENT];
  };
  
////////////////////////////////////////////////////////////////////////////////
  //! Event of keys
  class KeyState : public InputState {
    
    //! 
  public:
    inline bool isKeyPressed(int key) const { return press_vector_[key]; }
    inline bool isLastKeyPressed() const { return last_pressed_; }
    inline int getLastKey() const { return last_key_; }
    
    //! Key codes, they're mostly ASCII
    //! \fixme more
    enum Keys {
      KeyBackspace = 8,
      KeyEnter = 13,
      KeySpace = ' ',
      KeyEsc = 27,
      KeyPageUp = 33,
      KeyPageDown = 34,
      KeyLeft = 37,
      KeyUp = 38,
      KeyRight = 39,
      KeyDown = 40,
      KeyA = 'A',
      //! modifiers are keys too
      KeyShift = 128,
      KeyAlt,
      KeyCtrl,
      KeyMax
    };
    
  public:
    KeyState() : last_key_(0), last_pressed_(false) { reset(); }
    
    inline void key(int key, bool pressed, u32 time)
    {
      press_vector_[key] = pressed;
      last_pressed_ = pressed;
      last_key_ = key;
      update(time);
    }
    
    void reset()
    {
      for (int i = 0; i < KeyMax; ++i)
        press_vector_[i] = false;
    }
    
  private:
    int last_key_;
    bool last_pressed_;
    
    //! \todo bit field
    bool press_vector_[KeyMax];
  };
  
  //! \todo stated: key, scroll, text input, orientation
  
} // namespace kapusha