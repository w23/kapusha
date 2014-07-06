#include <X11/Xlib.h>
#include <GL/glx.h>

#include "kapusha/app/event.h"

static inline KPKey x11_keyevent_to_keytype(XKeyEvent *event) {
  int keysym = XLookupKeysym(event, 0);
  switch (keysym) {
#define XKKK(xk,kk) case XK_##xk: return KPKey##kk
    XKKK(BackSpace, Backspace); XKKK(Tab,Tab); XKKK(Return, Enter); XKKK(space, Space);
    XKKK(Escape, Esc);
    XKKK(Page_Up, PageUp); XKKK(Page_Down, PageDown);
    XKKK(Left, Left); XKKK(Up, Up); XKKK(Right, Right); XKKK(Down, Down);
    XKKK(comma, Comma); XKKK(minus, Minus); XKKK(period, Dot); XKKK(slash, Slash);
    XKKK(equal, Equal);
    XKKK(Delete, Del); XKKK(Insert, Ins); XKKK(Home, Home); XKKK(End, End);
    XKKK(asterisk, KeypadAsterisk);
    XKKK(0, 0); XKKK(1, 1); XKKK(2, 2); XKKK(3, 3); XKKK(4, 4); XKKK(5, 5);
    XKKK(6, 6); XKKK(7, 7); XKKK(8, 8); XKKK(9, 9);
    XKKK(a, A); XKKK(b, B); XKKK(c, C); XKKK(d, D); XKKK(e, E); XKKK(f, F); XKKK(g, G);
    XKKK(h, H); XKKK(i, I); XKKK(j, J); XKKK(k, K); XKKK(l, L); XKKK(m, M); XKKK(n, N);
    XKKK(o, O); XKKK(p, P); XKKK(q, Q); XKKK(r, R); XKKK(s, S); XKKK(t, T); XKKK(u, U);
    XKKK(v, V); XKKK(w, W); XKKK(x, X); XKKK(y, Y); XKKK(z, Z);
    XKKK(KP_Add, padPlus); XKKK(KP_Subtract, padMinus);
    XKKK(F1, F1); XKKK(F2, F2); XKKK(F3, F3); XKKK(F4, F4); XKKK(F5, F5);
    XKKK(F6, F6); XKKK(F7, F7); XKKK(F8, F8); XKKK(F9, F9); XKKK(F10, F10);
    XKKK(F11, F11); XKKK(F12, F12);
    XKKK(Alt_L, LeftAlt); XKKK(Control_L, LeftCtrl); XKKK(Meta_L, LeftMeta);
    XKKK(Super_L, LeftSuper); XKKK(Shift_L, LeftShift); XKKK(Alt_R, RightAlt);
    XKKK(Control_R, RightCtrl); XKKK(Meta_R, RightMeta); XKKK(Super_R, RightSuper);
    XKKK(Shift_R, RightShift); XKKK(Caps_Lock, Capslock);
#undef XKKK
  }
  KP_L("Unknown keysym 0x%04x", keysym);
  return KPKeyUnknown;
}

static inline void x11_handle_key(XKeyEvent *xkey,
    KPinput_key_t *keys, KPevent_handler_t *handler) {
  KPevent_key_t event;
  event.key = x11_keyevent_to_keytype(xkey);
  if (event.key == KPKeyUnknown)
    return;

  event.pressed = (xkey->type == KeyPress);
  keys->keys[event.key] = event.pressed;

  if (handler->func == NULL)
    return;

  event.type = KPEventClass_Keyboard;
  event.timestamp = xkey->time;
  event.device_index = 0;
  event.state = keys;
  handler->func(handler->param, &event);
}

static inline void x11_handle_pointer_button(XButtonEvent *xbutton,
    KPinput_pointer_t *pointer, KPevent_handler_t *handler) {
  KPevent_pointer_t event;

  switch (xbutton->button) {
    case Button1: event.button = KPInputPointer_LeftButton; break;
    case Button2: event.button = KPInputPointer_MiddleButton; break;
    case Button3: event.button = KPInputPointer_RightButton; break;
    default: return;
  }

  if (xbutton->type == ButtonPress) {
    event.pressed = 1;
    pointer->pointers[0].buttons |= event.button;
  } else {
    event.pressed = 0;
    pointer->pointers[0].buttons ^= event.button;
  }

  if (handler->func == NULL)
    return;

  event.type = KPEventClass_Pointer;
  event.timestamp = xbutton->time;
  event.device_index = 0;
  event.pointer_index = 0;
  event.d_pix = KP_vec2i(0, 0);
  event.state = pointer;
  handler->func(handler->param, &event);
}

static inline void x11_handle_pointer_motion(XMotionEvent *xmotion,
    KPinput_pointer_t *pointer, KPevent_handler_t *handler) {
  KPevent_pointer_t event;

  KPvec2i prev_pos_pix = pointer->pointers[0].pos_pix;

  /* TODO grabbed mode */

  pointer->pointers[0].pos_pix.x = xmotion->x;
  pointer->pointers[0].pos_pix.y = xmotion->y;

  if (handler->func == NULL)
    return;

  event.type = KPEventClass_Pointer;
  event.timestamp = xmotion->time;
  event.device_index = 0;
  event.pointer_index = 0;
  event.d_pix.x = xmotion->x - prev_pos_pix.x;
  event.d_pix.y = xmotion->y - prev_pos_pix.y;
  event.button = 0;
  event.pressed = 0;
  event.state = pointer;
  handler->func(handler->param, &event);
}
