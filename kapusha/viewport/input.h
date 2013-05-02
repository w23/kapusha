#pragma once
#include "../core.h"
#include "../core/limits.h"
#include "../math.h"

namespace kapusha {
  //! Basic input state with timestamp
  class InputState {
  protected:
    inline void accumulate(u32 time) { timestampDelta_ += time - timestamp_, timestamp_ = time; }
    inline void update(u32 time) { timestampDelta_ = 0, accumulate(time); }
    inline u32 getTimestamp() const { return timestamp_; }
    inline u32 getDelta() const { return timestampDelta_; }
    inline void resetDelta() { timestampDelta_ = 0; }
  private:
    u32 timestamp_;
    u32 timestampDelta_;
  }; // class InputState
////////////////////////////////////////////////////////////////////////////////
  //! State of an input by pointers
  class PointerState : public InputState {
  public:
    //! Single pointer
    struct Pointer {
      enum Flags {
        None = 0, //! pointer is inactive
        Moved = 1, //! pointer has moved (event flag)
        Pressed = 2, //! touch down or 
        LeftButton = Pressed, //! left mouse button
        RightButton = 4,
        MiddleButton = 8,
        Cancelled = 16, //! pointer was cancelled (event flag)
        AnyButton = LeftButton | RightButton | MiddleButton,
        _Event = Moved | Cancelled,
      };
      inline bool isPressed() const { return (flags_ & AnyButton) != 0; }
      inline bool isLeftPressed() const { return (flags_ & LeftButton) != 0; }
      inline bool isRightPressed() const { return (flags_ & RightButton) != 0; }
      inline bool isMiddlePressed() const { return (flags_ & MiddleButton) != 0;}
      inline bool wasPressed() const { return isPressed() && (flagsChanged_ & AnyButton); }
      inline bool wasUnpressed() const { return !isPressed() && (flagsChanged_ & AnyButton); }
      inline bool wasMoved() const { return (flags_ & Moved) != 0; }
      inline bool wasCancelled() const { return (flags_ & Cancelled) != 0; }
      inline vec2f getPosition() const { return point_; }
      inline vec2f getDelta() const { return pointDelta_; }
    protected:
      friend class PointerState;
      int flags_, flagsChanged_;
      vec2f point_, pointDelta_; //! in normalized viewport coordinates
      Pointer() : flags_(0), flagsChanged_(0), point_(0), pointDelta_(0) {}
      inline void prepare() { flags_ ^= _Event; }
      inline void clear() { flags_ = 0; }
      inline void updateFlags(int flags_add, int flags_remove = 0) {
        int newflags = (flags_ | flags_add) ^ flags_remove;
        flagsChanged_ = flags_ ^ newflags, flags_ = newflags;
      }
      void updateWithDelta(vec2f delta, int flags_add, int flags_remove = 0) {
        pointDelta_ = delta, point_ += delta;
        updateFlags(flags_add, flags_remove);
      }
      void updateWithPosition(vec2f position, int flags_add, int flags_remove = 0) {
        pointDelta_ = position - point_, point_ = position;
        updateFlags(flags_add, flags_remove);
      }
    }; // struct Pointer
  public: // IViewport user interface
    inline const Pointer& main() const { return pointers_[0]; }
    inline bool isPressed() const { return main().isPressed(); }
    inline bool isLeftPressed() const { return main().isLeftPressed(); }
    inline bool isRightPressed() const { return main().isRightPressed(); }
    inline bool isMiddlePressed() const { return main().isMiddlePressed(); }
    inline bool wasPressed() const { return main().wasPressed(); }
    inline bool wasUnpressed() const { return main().wasUnpressed(); }
    inline bool wasCancelled() const { return main().wasCancelled(); }
  protected: // implementers
    PointerState()
      : flagsCombined_(0), flagsChanged_(0), scale_(1.f), shift_(0.f) {}
    inline void resizeViewport(vec2f bottomLeft, vec2f topRight) {
      scale_ = vec2f(2.f) / (topRight - bottomLeft); shift_ = bottomLeft;
    }
    void mouseMoveTo(vec2f w_to, u32 time);
    void mouseMoveBy(vec2f w_by, u32 time);
    void mouseDown(int button, u32 time);
    void mouseUp(int button, u32 time);
    //! (sys impl) call this before updating the state with new events
    void beginUpdate();
    void pointerMoveTo(int index, vec2f w_at, int flags_add, int flags_remove);
    void pointerMoveBy(int index, vec2f w_by, int flags_add, int flags_remove);
    //! (sys impl) call this after all the new events were consumed
    void endUpdate(u32 time);
    int flagsCombined_;
    int flagsChanged_;
    //! \todo mat3f windowToViewport_; isntead. PREREQ: change mat order to less of an opengl one
    vec2f scale_, shift_;
    Pointer pointers_[KAPUSHA_MAX_POINTERS_IN_EVENT];
  }; // class PointerState
////////////////////////////////////////////////////////////////////////////////
  //! Event of keys
  class KeyState : public InputState {
  public:
    inline bool isKeyPressed(int key) const { return press_vector_[key]; }
    inline bool isLastKeyPressed() const { return press_vector_[last_key_]; }
    inline int getLastKey() const { return last_key_; }

    // useful shortcuts
    inline bool isShiftPressed() const {
      return press_vector_[KeyLeftShift] || press_vector_[KeyRightShift];
    }
    
    //! Key codes, they're mostly ASCII
    //! \fixme more
    //! \fixme SDL_keysym seems to have the most appropriate mapping. should i adopt it?
    //! \fixme this is a total mess
    enum Keys {
      KeyUnknown = 0,
      KeyBackspace = 8,
      KeyTab = 9,
      KeyEnter = 13,
      KeySpace = ' ',
      KeyEsc = 27,
      KeyPageUp = 33,
      KeyPageDown = 34,
      KeyLeft = 37,
      KeyUp = 38,
      KeyRight = 39,
      KeyDown = 40,
      KeyComma = 44,  // ','
      KeyMinus = 45,  // '-'
      KeyDot = 46,    // '.'
      KeySlash = 47,  // '/'
      Key0 = '0',
      Key1 = '1',
      Key2 = '2',
      Key3 = '3',
      Key4 = '4',
      Key5 = '5',
      Key6 = '6',
      Key7 = '7',
      Key8 = '8',
      Key9 = '9',
      KeyEqual = 61, // '='
      KeyA = 'A',
      KeyB = 'B',
      KeyC = 'C',
      KeyD = 'D',
      KeyE = 'E',
      KeyF = 'F',
      KeyG = 'G',
      KeyH = 'H',
      KeyI = 'I',
      KeyJ = 'J',
      KeyK = 'K',
      KeyL = 'L',
      KeyM = 'M',
      KeyN = 'N',
      KeyO = 'O',
      KeyP = 'P',
      KeyQ = 'Q',
      KeyR = 'R',
      KeyS = 'S',
      KeyT = 'T',
      KeyU = 'U',
      KeyV = 'V',
      KeyW = 'W',
      KeyX = 'X',
      KeyY = 'Y',
      KeyZ = 'Z',
      //! modifiers are keys too
      KeyDel = 127,
      KeyIns,
      KeyLeftAlt,
      KeyLeftCtrl,
      KeyLeftMeta,
      KeyLeftShift,
      KeyRightAlt,
      KeyRightCtrl,
      KeyRightMeta,
      KeyRightShift,
      KeyOpt = KeyLeftAlt,
      KeyF1, KeyF2, KeyF3, KeyF4, KeyF5, KeyF6, KeyF7, KeyF8, KeyF9, KeyF10,
      KeyF11, KeyF12,
      KeyKeypadAsterisk,
      KeyHome, KeyEnd,
      KeyCapslock,
      KeyMax = 256
    };
    
  public:
    KeyState() : last_key_(0) { reset(); }
    
    inline bool key(unsigned key, bool pressed, u32 time) {
      KP_ASSERT(key < KeyMax);
      if (key == KeyUnknown) return false;
      press_vector_[key] = pressed;
      last_key_ = key;
      update(time);
      return true;
    }
    
    void reset() {
      for (int i = 0; i < KeyMax; ++i)
        press_vector_[i] = false;
    }
    
  private:
    int last_key_;
    //! \fixme bit field
    bool press_vector_[KeyMax];
  };
  
  //! \todo stated: key, scroll, text input, orientation, leapmotion
} // namespace kapusha
