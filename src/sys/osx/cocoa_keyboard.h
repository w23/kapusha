#import <Cocoa/Cocoa.h>
#import "kapusha/app.h"

typedef struct KP__input_keyboard_t {
  KPinput_t I;
  KPinput_register_t keys[KPKey_count];
  KPu32 evt_serial;
  KPmessage_queue_t evt_queue;
} KP__input_keyboard_t;

void kp__KeyboardKey(KP__input_keyboard_t *keyboard,
  KPKey key, int down, KPtime_ns time);

typedef struct KP__cocoa_keyboard_t {
  KP__input_keyboard_t K;
} KP__cocoa_keyboard_t, *KP__cocoa_keyboard_o;

KP__cocoa_keyboard_o kp__CocoaKeyboardCreate();
void kp__CocoaKeyboardEvent(KP__cocoa_keyboard_o keyboard,
  NSEvent *event, KPtime_ns time);