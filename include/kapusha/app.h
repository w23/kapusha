#pragma once

#include <kapusha/core.h>

int kpuserAppCreate(int argc, const char *argv[]);
int kpuserAppDestroy();

void kpAppSetEventQueue(KPmessage_queue_t *queue);

typedef struct KPoutput_t { KP_O;
  enum {
    KPOutputVideo/*,
    KPOutputAudio*/
  } type;
  const char *name;
} KPoutput_t, *KPoutput_o;

enum {
  KPOutputSelectorType, /* KPOutputVideo, KPOutputAudio, default == any*/
  KPOutputSelectorFlags,
  KPOutputSelector_End = 0
} KPOutputSelectors;

KPiterable_o kpOutputsSelect(KPuptr *selectors);

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

KPiterable_o kpInputSelect(KPuptr *selectors);
