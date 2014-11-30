#pragma once

#include <kapusha/core.h>

#ifdef __cplusplus
extern "C" {
#endif

/* should be implemented by api user */
/* Entry point to dedicated application thread (not main()) */
int kpuserAppRun(int argc, const char **argv);

void kpAppDestroy(int exit_code /* = 0 for success */);

typedef struct KPoutput_t { KP_O;
  enum {
    KPOutputVideo/*,
    KPOutputAudio*/
  } type;
  const char *name;
} KPoutput_t, *KPoutput_o;

typedef struct KPcolorspace_t {
  KPf32 red_x, red_y;
  KPf32 green_x, green_y;
  KPf32 blue_x, blue_y;
  KPf32 white_x, white_y;
  KPf32 gamma;
} KPcolorspace_t;

enum {
  KPVideoOutputCombined = 0x0001,
  KPVideoOutputPrimary = 0x0002,
  KPVideoOutputOculus = 0x0003,
  KPVideoOutputActive = 0x0004
} KPVideoOutputFlags;

typedef struct KPoutput_video_t { KPoutput_t header;
  int width, height;
  int width_mm, height_mm;
  KPf32 vppmm, hppmm;
  KPtime_ns frame_delta;
  KPu32 flags;
  KPcolorspace_t colorspace;
} KPoutput_video_t, *KPoutput_video_o;

enum {
  KPOutputSelector_End = 0,
  KPOutputSelectorType, /* KPOutputVideo, KPOutputAudio, default == any */
  KPOutputSelectorFlags
} KPOutputSelectors;

typedef struct KPinput_register_t {
  union {
    KPs32 iv;
    KPu32 uv;
    KPf32 fv;
  };
} KPinput_register_t;

typedef struct KPinput_t { KP_O;
  enum {
    KPInputKeyboard,
    KPInputPointer/*,
    KPInputMIDI,
    KPInputHeadTracker,
    KPInputGamepad,
    KPInputVideo,
    KPInputAudio*/
  } type;
  const char *name;
  KPu32 nregisters;
  KPinput_register_t *registers;
} KPinput_t, *KPinput_o;

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

enum {
  KPInputSelectorType,
  KPInputSelectorAssociatedOutput,
  KPInputSelector_End = 0
};

KPsize kpOutputsSelect(KPuptr *selectors, KPoutput_o *outputs, KPsize max);
KPsize kpInputsSelect(KPuptr *selectors, KPinput_o *inputs, KPsize max);

#ifdef __cplusplus
} // extern "C"
#endif
