#pragma once

#include <kapusha/core.h>

#ifdef __cplusplus
extern "C" {
#endif

/* should be implemented by api user */
/* Application starting point */
int kpuserAppCreate(int argc, const char *argv[]);
int kpuserAppDestroy();

void kpAppSetEventQueue(KPmessage_queue_t *queue);
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
  KPOutputSelectorType, /* KPOutputVideo, KPOutputAudio, default == any*/
  KPOutputSelectorFlags,
  KPOutputSelector_End = 0
} KPOutputSelectors;

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
} KPinput_t, *KPinput_o;

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
