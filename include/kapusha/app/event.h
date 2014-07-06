#pragma once

#include "kapusha/core.h"
#include "kapusha/math.h"
#include "input.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  KPEventClass_Keyboard = 0x0100,
  KPEventClass_Pointer  = 0x0101

  /* TODO aggregate of keys and values */
} KPEventClass;

typedef struct {
  int type;
  KPtime_ms timestamp;
  int device_index;
  KPKey key;
  int pressed;
  KPinput_key_t *state;
} KPevent_key_t;

typedef struct {
  int type;
  KPtime_ms timestamp;
  int device_index;
  int pointer_index;
  KPvec2i d_pix;
  KPu32 button;
  int pressed;
  KPinput_pointer_t *state;
} KPevent_pointer_t;

#ifdef __cplusplus
} // extern "C"
#endif
