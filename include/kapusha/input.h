#pragma once

#include <kapusha/core.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
  KPInputKeyboardKey = 0x0100
};

typedef struct KPinput_reg_t {
  union {
    KPu32 u;
    KPs32 s;
    KPf32 f;
  } v;
} KPinput_reg_t;

typedef struct KPinput_reg_state_t {
  KPsize nreg;
  KPinput_reg_t *reg;
} KPinput_registers_state_t;

typedef struct KPinput_reg_event_data_t {
  KPsize nchanged;
  KPsize *changed;
  KPinput_registers_state_t state;
} KPinput_reg_event_data_t;

typedef enum {
  KPKeyUnknown = 0, KPKeyBackspace = 8, KPKeyTab = 9, KPKeyEnter = 13,
  KPKeySpace = ' ', KPKeyEsc = 27, KPKeyPageUp = 33, KPKeyPageDown = 34,
  KPKeyLeft = 37, KPKeyUp = 38, KPKeyRight = 39, KPKeyDown = 40,
  KPKeyComma = ',', KPKeyMinus = '-', KPKeyDot = '.', KPKeySlash = '/',
  KPKey0 = '0', KPKey1, KPKey2, KPKey3, KPKey4, KPKey5, KPKey6, KPKey7, KPKey8,
  KPKey9, KPKeyEqual = '=',
  KPKeyA = 'A', KPKeyB = 'B', KPKeyC = 'C', KPKeyD = 'D', KPKeyE = 'E',
  KPKeyF = 'F', KPKeyG = 'G', KPKeyH = 'H', KPKeyI = 'I', KPKeyJ = 'J',
  KPKeyK = 'K', KPKeyL = 'L', KPKeyM = 'M', KPKeyN = 'N', KPKeyO = 'O',
  KPKeyP = 'P', KPKeyQ = 'Q', KPKeyR = 'R', KPKeyS = 'S', KPKeyT = 'T',
  KPKeyU = 'U', KPKeyV = 'V', KPKeyW = 'W', KPKeyX = 'X', KPKeyY = 'Y',
  KPKeyZ = 'Z',
  KPKeypadPlus, KPKeypadMinus,
  /* modifiers are keys too */
  KPKeyDel = 127, KPKeyIns,
  KPKeyLeftAlt, KPKeyLeftCtrl, KPKeyLeftMeta, KPKeyLeftSuper, KPKeyLeftShift,
  KPKeyRightAlt, KPKeyRightCtrl, KPKeyRightMeta, KPKeyRightSuper, KPKeyRightShift,
  KPKeyOpt = KPKeyLeftAlt,
  KPKeyF1, KPKeyF2, KPKeyF3, KPKeyF4, KPKeyF5, KPKeyF6,
  KPKeyF7, KPKeyF8, KPKeyF9, KPKeyF10, KPKeyF11, KPKeyF12,
  KPKeyKeypadAsterisk,
  KPKeyHome, KPKeyEnd,
  KPKeyCapslock,
  KPKey_count = 256
} KPKey;

typedef struct KPinput_keyboard_state_t {
  KPinput_reg_t keys[KPKey_count];
} KPinput_keyboard_state_t;

typedef struct KPinput_keyboard_event_data_t {
  KPinput_reg_event_data_t reg;
  KPsize changed;
  KPinput_keyboard_state_t state;
} KPinput_keyboard_event_data_t;

#ifdef __cplusplus
} // extern "C"
#endif
