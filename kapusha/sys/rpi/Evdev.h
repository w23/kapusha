#pragma once
#include <linux/input.h>
#include <kapusha/viewport/input.h>

namespace kapusha {

  class IViewport;
  class Evdev;

  enum EventProcessingState {
    EventIgnored = 0,
    EventProcessing,
    EventComplete
  };

  //! \todo evdev multitouch (someday, when i get some hw to test it on)
  class EvdevPointerState : public PointerState {
  protected:
    friend class Evdev;
    inline EvdevPointerState(vec2i vpsize);
    inline void setRelative(bool only) {
      relativeOnly_ = only;
    }
    EventProcessingState process(u32 kptime, const input_event &e);

  private:
    EventProcessingState state_;
    bool relativeOnly_;
  }; // class EvdevPointerState

  class EvdevKeyState : public KeyState {
  protected:
    friend class Evdev;
    EvdevKeyState() : state_(EventComplete) {}
    EventProcessingState process(u32 kptime, const input_event &e);

  private:
#define EVDEV_MAXKEY KEY_DELETE // 111
    static const Keys s_keymap_[EVDEV_MAXKEY+1];
    EventProcessingState state_;
  };

  //! \todo EvdevScrollState

  class Evdev {
  public:
    Evdev(IViewport *viewport, vec2i vpsize,
        const char *file_mouse,
        const char *file_kbd);
    ~Evdev();
    void run(bool block);

    inline void setRelativeOnly(bool ronly) {
      pointer_.setRelative(ronly);
    }

    const PointerState& pointerState() const { return pointer_; }
    const KeyState& keyState() const { return key_; }

  private:
    IViewport *viewport_;
    int fileMouse_;
    int fileKeyboard_;
    int maxFds_;
    EvdevPointerState pointer_;
    EvdevKeyState key_;
    //EvdevScrollState scroll_;

    u32 localtime(const timeval &tv) const;
    u32 localSecOffset_;
  };

} // namespace kapusha
