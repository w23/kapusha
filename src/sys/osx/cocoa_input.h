#import <Cocoa/Cocoa.h>
#import "kapusha/input.h"

typedef struct KP__input_keyboard_t {
  KPuptr queue_serial;
  void *queue_origin;
  KPmessage_user_t queue_user;
  KPmessage_queue_p queue;
  KPallocator_p message_pool;
  KPinput_keyboard_state_t state;
} KP__input_keyboard_t;

void kp__KeyboardInit(KP__input_keyboard_t *);
void kp__KeyboardKey(KP__input_keyboard_t *, KPKey, int down, KPtime_ns time);

typedef struct KP__cocoa_keyboard_t {
  KP__input_keyboard_t kbd;
} KP__cocoa_keyboard_t;

void kp__CocoaKeyboardInit(KP__cocoa_keyboard_t *keyboard);
void kp__CocoaKeyboardEvent(KP__cocoa_keyboard_t *keyboard,
  NSEvent *event, KPtime_ns time);

typedef struct KP__cocoa_mouse_t {
} KP__cocoa_mouse_t;

void kp__CocoaMouseInit(KP__cocoa_mouse_t *mouse);
void kp__CocoaMouseEvent(KP__cocoa_mouse_t *mouse, NSWindow *window,
  NSEvent *event, KPtime_ns time);