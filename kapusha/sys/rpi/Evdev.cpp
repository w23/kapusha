#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>

#include "../../core/Core.h"
#include "../../core/IViewport.h"
#include "Evdev.h"

namespace kapusha {

  EvdevPointerState::EvdevPointerState(vec2i vpsize)
    : state_(EventComplete)
    , kpix_(vec2f(1.f, -1.f) / vec2f(vpsize))
    , relativeOnly_(false)
  {
  }

  EventProcessingState
  EvdevPointerState::process(u32 kptime, const input_event &e)
  {
    //L("\tmouse state == %d", state_);
    if (state_ == EventComplete) beginUpdate();

    switch (e.type)
    {
      case EV_SYN:
        if (e.code != SYN_REPORT || state_ != EventProcessing)
          return EventIgnored;
        accumulate(kptime);
        endUpdate();
        return state_ = EventComplete;

      case EV_KEY:
        {
          int btn = 0;
          switch (e.code){
            case BTN_LEFT:
              btn = Pointer::LeftButton;
              break;
            case BTN_RIGHT:
              btn = Pointer::RightButton;
              break;
            case BTN_MIDDLE:
              btn = Pointer::MiddleButton;
              break;
            default: return EventIgnored;
          }
          if (e.value == 0)
            pointers_[0].flags &= ~btn;
          else
            pointers_[0].flags |= btn;
          pointers_[0].change |= btn;
        }
        return state_ = EventProcessing;

      case EV_REL:
        {
          vec2f d(0);
          if (e.code == REL_X)
            d.x = kpix_.x * e.value;
          else if (e.code == REL_Y)
            d.y = kpix_.y * e.value;
          else return EventIgnored;
          
          pointers_[0].flags |= Pointer::Move;
          pointers_[0].change |= Pointer::Move;

          if (relativeOnly_)
          {
            pointers_[0].movement += d;
            pointers_[0].point = vec2f(0);
          } else {
            vec2f newpos = (pointers_[0].point + d).clamp(-1.f, 1.f);
            pointers_[0].movement += newpos - pointers_[0].point;
            pointers_[0].point = newpos;
          }
        }
        return state_ = EventProcessing;

      default: return EventIgnored;
    }
  }

////////////////////////////////////////////////////////////////////////////////

  const KeyState::Keys EvdevKeyState::s_keymap_[EVDEV_MAXKEY+1] = {
    KeyUnknown, // KEY_RESERVED	0
    KeyEsc,     // KEY_ESC      1
    Key1,       // KEY_1        2
    Key2,       // KEY_2        3
    Key3,       // KEY_3        4
    Key4,       // KEY_4        5
    Key5,       // KEY_5        6
    Key6,       // KEY_6        7
    Key7,       // KEY_7        8
    Key8,       // KEY_8        9
    Key9,       // KEY_9        10
    Key0,       // KEY_0        11
    KeyMinus,   // KEY_MINUS    12
    KeyEqual,   // KEY_EQUAL    13
    KeyBackspace, // KEY_BACKSPACE  14
    KeyTab,       // KEY_TAB    15
    KeyQ,       // KEY_Q        16
    KeyW,       // KEY_W        17
    KeyE,       // KEY_E        18
    KeyR,       // KEY_R        19
    KeyT,       // KEY_T        20
    KeyY,       // KEY_Y        21
    KeyU,       // KEY_U        22
    KeyI,       // KEY_I        23
    KeyO,       // KEY_O        24
    KeyP,       // KEY_P        25
    KeyUnknown, // KEY_LEFTBRACE  26
    KeyUnknown, // KEY_RIGHTBRACE 27
    KeyEnter,   // KEY_ENTER	28
    KeyLeftCtrl, // KEY_LEFTCTRL  29
    KeyA,       // KEY_A        30
    KeyS,       // KEY_S        31
    KeyD,       // KEY_D        32
    KeyF,       // KEY_F        33
    KeyG,       // KEY_G        34
    KeyH,       // KEY_H        35
    KeyJ,       // KEY_J        36
    KeyK,       // KEY_K        37
    KeyL,       // KEY_L        38
    KeyUnknown, // KEY_SEMICOLON  39
    KeyUnknown, // KEY_APOSTROPHE  40
    KeyUnknown, // KEY_GRAVE    41
    KeyLeftShift, // KEY_LEFTSHIFT 42
    KeyUnknown, // KEY_BACKSLASH 43
    KeyZ,       // KEY_Z        44
    KeyX,       // KEY_X        45
    KeyC,       // KEY_C        46
    KeyV,       // KEY_V        47
    KeyB,       // KEY_B        48
    KeyN,       // KEY_N        49
    KeyM,       // KEY_M        50
    KeyComma,   // KEY_COMMA    51
    KeyDot,     // KEY_DOT      52
    KeySlash,   // KEY_SLASH    53
    KeyRightShift,      // KEY_RIGHTSHIFT   54
    KeyKeypadAsterisk,  // KEY_KPASTERISK   55
    KeyLeftAlt, // KEY_LEFTALT  56
    KeySpace,   // KEY_SPACE    57
    KeyCapslock,  // KEY_CAPSLOCK   58
    KeyF1,      // KEY_F1       59
    KeyF2,      // KEY_F2       60
    KeyF3,      // KEY_F3       61
    KeyF4,      // KEY_F4       62
    KeyF5,      // KEY_F5       63
    KeyF6,      // KEY_F6       64
    KeyF7,      // KEY_F7       65
    KeyF8,      // KEY_F8       66
    KeyF9,      // KEY_F9       67
    KeyF10,     // KEY_F10      68
    KeyUnknown, // KEY_NUMLOCK  69
    KeyUnknown, // KEY_SCROLLLOCK   70
    KeyUnknown, // KEY_KP7      71
    KeyUnknown, // KEY_KP8      72
    KeyUnknown, // KEY_KP9      73
    KeyUnknown, // KEY_KPMINUS  74
    KeyUnknown, // KEY_KP4      75
    KeyUnknown, // KEY_KP5      76
    KeyUnknown, // KEY_KP6      77
    KeyUnknown, // KEY_KPPLUS   78
    KeyUnknown, // KEY_KP1      79
    KeyUnknown, // KEY_KP2      80
    KeyUnknown, // KEY_KP3      81
    KeyUnknown, // KEY_KP0      82
    KeyUnknown, // KEY_KPDOT    83
    KeyUnknown, // ???          84
    KeyUnknown, // KEY_ZENKAKUHANKAKU   85
    KeyUnknown, // KEY_102ND    86
    KeyF11,     // KEY_F11      87
    KeyF12,     // KEY_F12      88
    KeyUnknown, // KEY_RO       89
    KeyUnknown, // KEY_KATAKANA 90
    KeyUnknown, // KEY_HIRAGANA 91
    KeyUnknown, // KEY_HENKAN   92
    KeyUnknown, // KEY_KATAKANAHIRAGANA   93
    KeyUnknown, // KEY_MUHENKAN 94
    KeyUnknown, // KEY_KPJPCOMMA  95
    KeyUnknown, // KEY_KPENTER  96
    KeyRightCtrl, // KEY_RIGHTCTRL  97
    KeyUnknown, // KEY_KPSLASH  98
    KeyUnknown, // KEY_SYSRQ    99
    KeyRightAlt, // KEY_RIGHTALT  100
    KeyUnknown, // KEY_LINEFEED   101
    KeyHome,    // KEY_HOME     102
    KeyUp,      // KEY_UP       103
    KeyPageUp,  // KEY_PAGEUP   104
    KeyLeft,    // KEY_LEFT     105
    KeyRight,   // KEY_RIGHT    106
    KeyEnd,     // KEY_END      107
    KeyDown,    // KEY_DOWN     108
    KeyPageDown, // KEY_PAGEDOWN  109
    KeyIns,     // KEY_INSERT   110
    KeyDel      // KEY_DELETE   111
  };

  EventProcessingState
  EvdevKeyState::process(u32 kptime, const input_event &e)
  {
    switch (e.type)
    {
      case EV_KEY:
        if (e.code > EVDEV_MAXKEY || e.value == 2)
          return EventIgnored;

        if (key(s_keymap_[e.code], e.value, kptime))
          return state_ = EventComplete;

      default: return EventIgnored;
    }
  }

////////////////////////////////////////////////////////////////////////////////

  Evdev::Evdev(IViewport *viewport, vec2i vpsize,
               const char *file_mouse,
               const char *file_kbd)
    : viewport_(viewport)
    , fileMouse_(open(file_mouse, O_RDONLY))
    , fileKeyboard_(open(file_kbd, O_RDONLY))
    , maxFds_(1 + ((fileMouse_ > fileKeyboard_) ? fileMouse_ : fileKeyboard_))
    , pointer_(vpsize)
  {
    L("Using %s as mouse input", file_mouse);
    L("Using %s as keyboard input", file_kbd);
    KP_ASSERT(fileMouse_ != -1);
    KP_ASSERT(fileKeyboard_ != -1);

#if !DEBUG
    ioctl(fileMouse_, EVIOCGRAB, 1);
    ioctl(fileKeyboard_, EVIOCGRAB, 1);
#endif

    timeval tv;
    gettimeofday(&tv, 0);
    localSecOffset_ = tv.tv_sec;
  }

  Evdev::~Evdev()
  {
    ioctl(fileMouse_, EVIOCGRAB, 0);
    ioctl(fileKeyboard_, EVIOCGRAB, 0);
    close(fileMouse_);
    close(fileKeyboard_);
  }

  u32 Evdev::localtime(const timeval &tv) const
  {
    return (tv.tv_sec - localSecOffset_) * 1000 + tv.tv_usec / 1000;
  }

  void Evdev::run(bool block)
  {
#define N_EVENTS 32
    input_event event[N_EVENTS];

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fileMouse_, &fds);
    FD_SET(fileKeyboard_, &fds);

    timeval tv, *ptv = 0;
    if (!block)
    {
      tv.tv_sec = 0;
      tv.tv_usec = 0;
      ptv = &tv;
    }

    //! \fixme do while event queue is not empty
    
    int selval = select(maxFds_, &fds, 0, 0, ptv);
    KP_ASSERT(selval != -1);
    if (selval < 1) return;
    
    if (FD_ISSET(fileMouse_, &fds))
    {
      size_t nread = read(fileMouse_, event, sizeof(event));
      KP_ASSERT(nread > 0);
      KP_ASSERT(nread % sizeof(input_event) == 0);
      int nevents = nread / sizeof(input_event);
      //L("mouse events: %d", nevents);
      for (int i = 0; i < nevents; ++i)
      {
        EventProcessingState points = 
          pointer_.process(localtime(event[i].time), event[i]);

        //L("\tprocess = %d", points);
        //if (points == EventIgnored);
        //  L("\t%ld.%06ld: type=%u code=%u value=%d",
        //    event[i].time.tv_sec, event[i].time.tv_usec,
        //    event[i].type, event[i].code, event[i].value);

        if (points == EventComplete)
          viewport_->inputPointer(pointer_);

        //! \todo scroll (wheel) state
      }
    } // if FD_ISSET(mouse)

    if (FD_ISSET(fileKeyboard_, &fds))
    {
      size_t nread = read(fileKeyboard_, event, sizeof(event));
      KP_ASSERT(nread > 0);
      KP_ASSERT(nread % sizeof(input_event) == 0);
      int nevents = nread / sizeof(input_event);
      //L("keyboard events: %d", nevents);
      for (int i = 0; i < nevents; ++i)
      {
        EventProcessingState keys =
            key_.process(localtime(event[i].time), event[i]);
        
        //L("\tprocess = %d", keys);
        //if (keys == EventIgnored);
        //  L("\t%ld.%06ld: type=%u code=%u value=%d",
        //    event[i].time.tv_sec, event[i].time.tv_usec,
        //    event[i].type, event[i].code, event[i].value);

        if (keys == EventComplete)
          viewport_->inputKey(key_);

        //! \todo scroll (some keyboards have wheel too)
      }
    } // if FD_ISSET(keyboard)
  } // void Evdev::run(bool)

} // namespace kapusha
