#pragma once

#include "kapusha/core.h"
#include "kapusha/math.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  KPKeyUnknown = 0, KPKeyBackspace = 8, KPKeyTab = 9, KPKeyEnter = 13, KPKeySpace = ' ',
  KPKeyEsc = 27, KPKeyPageUp = 33, KPKeyPageDown = 34,
  KPKeyLeft = 37, KPKeyUp = 38, KPKeyRight = 39, KPKeyDown = 40,
  KPKeyComma = ',', KPKeyMinus = '-', KPKeyDot = '.', KPKeySlash = '/',
  KPKey0 = '0', KPKey1, KPKey2, KPKey3, KPKey4, KPKey5, KPKey6, KPKey7, KPKey8, KPKey9,
  KPKeyEqual = '=',
  KPKeyA = 'A', KPKeyB = 'B', KPKeyC = 'C', KPKeyD = 'D', KPKeyE = 'E', KPKeyF = 'F',
  KPKeyG = 'G', KPKeyH = 'H', KPKeyI = 'I', KPKeyJ = 'J', KPKeyK = 'K', KPKeyL = 'L',
  KPKeyM = 'M', KPKeyN = 'N', KPKeyO = 'O', KPKeyP = 'P', KPKeyQ = 'Q', KPKeyR = 'R',
  KPKeyS = 'S', KPKeyT = 'T', KPKeyU = 'U', KPKeyV = 'V', KPKeyW = 'W', KPKeyX = 'X',
  KPKeyY = 'Y', KPKeyZ = 'Z',
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

typedef struct {
  KPu8 keys[KPKey_count];
} KPinput_key_t;

#define KP_INPUT_MAX_POINTERS 1

enum {
  KPInputPointer_NoButton = 0x00,
  KPInputPointer_LeftButton = 0x01,
  KPInputPointer_MiddleButton = 0x02,
  KPInputPointer_RightButton = 0x04
};

typedef struct {
  struct {
    KPvec2i pos_pix;
    KPu32 buttons;
  } pointers[KP_INPUT_MAX_POINTERS];
} KPinput_pointer_t;

#ifdef __cplusplus
} // extern "C"
#endif
