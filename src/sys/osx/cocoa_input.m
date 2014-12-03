#import "cocoa_input.h"

static const int keymap[128] = {
  KPKeyA,       //0x00 = kVK_ANSI_A
  KPKeyS,       //0x01 = kVK_ANSI_S
  KPKeyD,       //0x02 = kVK_ANSI_D
  KPKeyF,       //0x03 = kVK_ANSI_F
  KPKeyH,       //0x04 = kVK_ANSI_H
  KPKeyG,       //0x05 = kVK_ANSI_G
  KPKeyZ,       //0x06 = kVK_ANSI_Z
  KPKeyX,       //0x07 = kVK_ANSI_X
  KPKeyC,       //0x08 = kVK_ANSI_C
  KPKeyV,       //0x09 = kVK_ANSI_V
  KPKeyUnknown, //0x0A = kVK_ISO_Section
  KPKeyB,       //0x0B = kVK_ANSI_B
  KPKeyQ,       //0x0C = kVK_ANSI_Q
  KPKeyW,       //0x0D = kVK_ANSI_W
  KPKeyE,       //0x0E = kVK_ANSI_E
  KPKeyR,       //0x0F = kVK_ANSI_R
  KPKeyY,       //0x10 = kVK_ANSI_Y
  KPKeyT,       //0x11 = kVK_ANSI_T
  KPKey1,       //0x12 = kVK_ANSI_1
  KPKey2,       //0x13 = kVK_ANSI_2
  KPKey3,       //0x14 = kVK_ANSI_3
  KPKey4,       //0x15 = kVK_ANSI_4
  KPKey6,       //0x16 = kVK_ANSI_6
  KPKey5,       //0x17 = kVK_ANSI_5
  KPKeyEqual,   //0x18 = kVK_ANSI_Equal
  KPKey9,       //0x19 = kVK_ANSI_9
  KPKey7,       //0x1A = kVK_ANSI_7
  KPKeyMinus,   //0x1B = kVK_ANSI_Minus
  KPKey8,       //0x1C = kVK_ANSI_8
  KPKey0,       //0x1D = kVK_ANSI_0
  KPKeyUnknown, //0x1E = kVK_ANSI_RightBracket
  KPKeyO,       //0x1F = kVK_ANSI_O
  KPKeyU,       //0x20 = kVK_ANSI_U
  KPKeyUnknown, //0x21 = kVK_ANSI_LeftBracket
  KPKeyI,       //0x22 = kVK_ANSI_I
  KPKeyP,       //0x23 = kVK_ANSI_P
  KPKeyEnter,   //0x24 = kVK_Return
  KPKeyL,       //0x25 = kVK_ANSI_L
  KPKeyJ,       //0x26 = kVK_ANSI_J
  KPKeyUnknown, //0x27 = kVK_ANSI_Quote
  KPKeyK,       //0x28 = kVK_ANSI_K
  KPKeyUnknown, //0x29 = kVK_ANSI_Semicolon
  KPKeyUnknown, //0x2A = kVK_ANSI_Backslash
  KPKeyComma,   //0x2B = kVK_ANSI_Comma
  KPKeySlash,   //0x2C = kVK_ANSI_Slash
  KPKeyN,       //0x2D = kVK_ANSI_N
  KPKeyM,       //0x2E = kVK_ANSI_M
  KPKeyDot,     //0x2F = kVK_ANSI_Period
  KPKeyTab,     //0x30 = kVK_Tab
  KPKeySpace,   //0x31 = kVK_Space
  KPKeyUnknown, //0x32 = kVK_ANSI_Grave
  KPKeyDel,     //0x33 = kVK_Delete
  KPKeyUnknown, //0x34
  KPKeyEsc,     //0x35 = kVK_Escape
  KPKeyUnknown, //0x36
  KPKeyUnknown, //0x37 = kVK_Command
  KPKeyLeftShift, //0x38 = kVK_Shift
  KPKeyUnknown, //0x39 = kVK_CapsLock
  KPKeyOpt,     //0x3A = kVK_Option
  KPKeyLeftCtrl,  //0x3B = kVK_Control
  KPKeyRightShift, //0x3C = kVK_RightShift
  KPKeyUnknown, //0x3D = kVK_RightOption
  KPKeyRightCtrl, //0x3E = kVK_RightControl
  KPKeyUnknown, //0x3F = kVK_Function
  KPKeyUnknown, //0x40 = kVK_F17
  KPKeyUnknown, //0x41 = kVK_ANSI_KeypadDecimal
  KPKeyUnknown, //0x42
  KPKeyUnknown, //0x43 = kVK_ANSI_KeypadMultiply
  KPKeyUnknown, //0x44
  KPKeyUnknown, //0x45 = kVK_ANSI_KeypadPlus
  KPKeyUnknown, //0x46
  KPKeyUnknown, //0x47 = kVK_ANSI_KeypadClear
  KPKeyUnknown, //0x48 = kVK_VolumeUp
  KPKeyUnknown, //0x49 = kVK_VolumeDown
  KPKeyUnknown, //0x4A = kVK_Mute
  KPKeyUnknown, //0x4B = kVK_ANSI_KeypadDivide
  KPKeyUnknown, //0x4C = kVK_ANSI_KeypadEnter
  KPKeyUnknown, //0x4D
  KPKeyUnknown, //0x4E = kVK_ANSI_KeypadMinus
  KPKeyUnknown, //0x4F = kVK_F18
  KPKeyUnknown, //0x50 = kVK_F19
  KPKeyUnknown, //0x51 = kVK_ANSI_KeypadEquals
  KPKeyUnknown, //0x52 = kVK_ANSI_Keypad0
  KPKeyUnknown, //0x53 = kVK_ANSI_Keypad1
  KPKeyUnknown, //0x54 = kVK_ANSI_Keypad2
  KPKeyUnknown, //0x55 = kVK_ANSI_Keypad3
  KPKeyUnknown, //0x56 = kVK_ANSI_Keypad4
  KPKeyUnknown, //0x57 = kVK_ANSI_Keypad5
  KPKeyUnknown, //0x58 = kVK_ANSI_Keypad6
  KPKeyUnknown, //0x59 = kVK_ANSI_Keypad7
  KPKeyUnknown, //0x5A = kVK_F20
  KPKeyUnknown, //0x5B = kVK_ANSI_Keypad8
  KPKeyUnknown, //0x5C = kVK_ANSI_Keypad9
  KPKeyUnknown, //0x5D = kVK_JIS_Yen
  KPKeyUnknown, //0x5E = kVK_JIS_Underscore
  KPKeyUnknown, //0x5F = kVK_JIS_KeypadComma
  KPKeyF5,      //0x60 = kVK_F5
  KPKeyF6,      //0x61 = kVK_F6
  KPKeyF7,      //0x62 = kVK_F7
  KPKeyF3,      //0x63 = kVK_F3
  KPKeyF8,      //0x64 = kVK_F8
  KPKeyF9,      //0x65 = kVK_F9
  KPKeyUnknown, //0x66 = kVK_JIS_Eisu
  KPKeyF11,     //0x67 = kVK_F11
  KPKeyUnknown, //0x68 = kVK_JIS_Kana
  KPKeyUnknown, //0x69 = kVK_F13
  KPKeyUnknown, //0x6A = kVK_F16
  KPKeyUnknown, //0x6B = kVK_F14
  KPKeyUnknown, //0x6C
  KPKeyF10,     //0x6D = kVK_F10
  KPKeyUnknown, //0x6E
  KPKeyF12,     //0x6F = kVK_F12
  KPKeyUnknown, //0x70
  KPKeyUnknown, //0x71 = kVK_F15
  KPKeyUnknown, //0x72 = kVK_Help
  KPKeyHome,    //0x73 = kVK_Home
  KPKeyPageUp,  //0x74 = kVK_PageUp
  KPKeyUnknown, //0x75 = kVK_ForwardDelete
  KPKeyF4,      //0x76 = kVK_F4
  KPKeyEnd,     //0x77 = kVK_End
  KPKeyF2,      //0x78 = kVK_F2
  KPKeyPageDown,//0x79 = kVK_PageDown
  KPKeyF1,      //0x7A = kVK_F1
  KPKeyLeft,    //0x7B = kVK_LeftArrow
  KPKeyRight,   //0x7C = kVK_RightArrow
  KPKeyDown,    //0x7D = kVK_DownArrow
  KPKeyUp,      //0x7E = kVK_UpArrow
  KPKeyUnknown  //0x7F
};

#define KP__SYS "Cocoa::keyboard"

void kp__KeyboardInit(KP__input_keyboard_t *keyboard) {
  keyboard->queue_serial = 0;
  keyboard->message_pool = kp_allocator_default;
  kpMemset(&keyboard->state, 0, sizeof(keyboard->state));
}

void kp__KeyboardKey(KP__input_keyboard_t *keyboard,
  KPKey key, int down, KPtime_ns time)
{
  KP_ASSERT(key >= 0);
  KP_ASSERT(key < KPKey_count);
  if (keyboard->state.keys[key].v.u == (!!down)) return;
  keyboard->state.keys[key].v.u = (!!down);
  
  KPmessage_carrier_t *carrier = kpNew(keyboard->message_pool,
    sizeof(KPmessage_carrier_t) + sizeof(KPinput_keyboard_event_data_t));
  
  KPinput_keyboard_event_data_t *event =
    (KPinput_keyboard_event_data_t*)(carrier + 1);
  event->changed = key;
  event->reg.changed = &event->changed;
  event->reg.nchanged = 1;
  event->state = keyboard->state;
  
  carrier->link.next = carrier->link.prev = 0;
  carrier->msg.data = event;
  carrier->msg.size = sizeof(*event);
  carrier->msg.type = KPInputKeyboardKey;
  carrier->msg.origin = keyboard->queue_origin;
  carrier->msg.param = key;
  carrier->msg.sequence = keyboard->queue_serial++;
  carrier->msg.timestamp = time;
  carrier->msg.user = keyboard->queue_user;
  kpMessageQueuePut(keyboard->queue, carrier);
}

void kp__CocoaKeyboardInit(KP__cocoa_keyboard_t *keyboard) {
  kp__KeyboardInit(&keyboard->kbd);
}

void kp__CocoaKeyboardEvent(KP__cocoa_keyboard_t *keyboard,
  NSEvent *event, KPtime_ns time)
{
  KP__input_keyboard_t *this = &keyboard->kbd;
  const int code = event.keyCode;
  if (code > 127) {
    KP__L("Unexpected keyCode %d", code);
    return;
  }

  int key = keymap[code];
  if (event.type != NSFlagsChanged) {
    kp__KeyboardKey(this, keymap[code], event.type == NSKeyDown, time);
  } else {
    int kbit;
    switch (key) {
      case KPKeyLeftShift:
        kbit = NSShiftKeyMask;
        break;
      case KPKeyLeftCtrl:
        kbit = NSControlKeyMask;
        break;
      case KPKeyLeftAlt:
        kbit = NSAlternateKeyMask;
        break;
      default:
        KP__L("Unknown key %d", key);
        return;
    }
    kp__KeyboardKey(this, keymap[code], event.modifierFlags & kbit, time);
  }
}

void kp__CocoaMouseInit(KP__cocoa_mouse_t *mouse) {
}

void kp__CocoaMouseEvent(KP__cocoa_mouse_t *mouse, NSWindow *window,
  NSEvent *event, KPtime_ns time)
{
}
