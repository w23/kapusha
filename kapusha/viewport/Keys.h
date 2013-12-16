#pragma once
#include "Input.h"

namespace kapusha {
  //! Event of keys
  class Keys : public Input {
  public:
    inline bool is_key_pressed(int key) const { return press_vector_[key]; }
    inline bool is_last_key_pressed() const { return press_vector_[last_key_]; }
    inline int last_key() const { return last_key_; }

    // useful shortcuts
    inline bool is_shift_pressed() const {
      return press_vector_[KeyLeftShift] || press_vector_[KeyRightShift];
    }
    inline bool is_ctrl_pressed() const {
      return press_vector_[KeyLeftCtrl] || press_vector_[KeyRightCtrl];
	}
    
    //! Key codes, they're mostly ASCII
    //! \fixme more
    //! \fixme SDL_keysym seems to have the most appropriate mapping. should i adopt it?
    //! \fixme this is a total mess
    enum Types {
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
	  KeypadPlus,
	  KeypadMinus,
      //! modifiers are keys too
      KeyDel = 127,
      KeyIns,
      KeyLeftAlt,
      KeyLeftCtrl,
      KeyLeftMeta,
      KeyLeftSuper,
      KeyLeftShift,
      KeyRightAlt,
      KeyRightCtrl,
      KeyRightMeta,
      KeyRightSuper,
      KeyRightShift,
      KeyOpt = KeyLeftAlt,
      KeyF1, KeyF2, KeyF3, KeyF4, KeyF5, KeyF6, KeyF7, KeyF8, KeyF9, KeyF10,
      KeyF11, KeyF12,
      KeyKeypadAsterisk,
      KeyHome, KeyEnd,
      KeyCapslock,
      KeyMax = 256
    };
    
  protected:
    Keys() : last_key_(0) { evt_reset(); }
    inline bool evt_key(unsigned key, bool pressed, u32 time) {
      KP_ASSERT(key < KeyMax);
      if (key == KeyUnknown) return false;
      press_vector_[key] = pressed;
      last_key_ = key;
      evt_update(time);
      return true;
    }
    
    void evt_reset() {
      for (int i = 0; i < KeyMax; ++i)
        press_vector_[i] = false;
    }
    
  private:
    int last_key_;
    //! \todo bit field
    bool press_vector_[KeyMax];
  }; // class Keys
} // namespace kapusha
