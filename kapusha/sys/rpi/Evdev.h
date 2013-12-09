#pragma once
#include <linux/input.h>
#include <kapusha/viewport/Pointers.h>
#include <kapusha/viewport/Keys.h>

namespace kapusha {

  class IViewport;
  class Evdev;

  enum EventProcessingState {
    EventIgnored = 0,
    EventProcessing,
    EventComplete
  };

  //! \todo evdev multitouch (someday, when i get some hw to test it on)
  class EvdevPointerState : public Pointers {
  protected:
    friend class Evdev;
    inline EvdevPointerState(vec2i vpsize);
    EventProcessingState process(u32 kptime, const input_event &e);

  private:
    EventProcessingState state_;
    bool track_cursor_;
  }; // class EvdevPointerState

  class EvdevKeyState : public Keys {
  protected:
    friend class Evdev;
    EvdevKeyState() : state_(EventComplete) {}
    EventProcessingState process(u32 kptime, const input_event &e);

  private:
#define EVDEV_MAXKEY KEY_DELETE // 111
    static const Types s_keymap_[EVDEV_MAXKEY+1];
    EventProcessingState state_;
  };

  //! \todo EvdevScrollState

  class Evdev {
  public:
    Evdev(vec2i vpsize, const char *file_mouse, const char *file_kbd);
    ~Evdev();
    inline void set_viewport(IViewport *viewport) { viewport_ = viewport; }
    void run(bool block);

    const Pointers& pointers() const { return pointer_; }
    const Keys& keys() const { return key_; }

  private:
    IViewport *viewport_;
    int file_mouse_;
    int file_keyboard_;
    int maxFds_;
    EvdevPointerState pointer_;
    EvdevKeyState key_;
    //EvdevScrollState scroll_;

    u32 localtime(const timeval &tv) const;
    u32 localSecOffset_;
  };

} // namespace kapusha
