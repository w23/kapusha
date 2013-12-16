#include "X11Keys.h"

namespace kapusha {
namespace sys {


static Keys::Types xkeyevent_to_keytype(const XKeyEvent *event) {
  int keysym = XLookupKeysym(const_cast<XKeyEvent*>(event), 0);
  switch (keysym) {
#define XKKK(xk,kk) case XK_##xk: return Keys::Key##kk
    XKKK(BackSpace, Backspace);
    XKKK(Tab,Tab);
    XKKK(Return, Enter);
    XKKK(space, Space);
    XKKK(Escape, Esc);
    XKKK(Page_Up, PageUp);
    XKKK(Page_Down, PageDown);
    XKKK(Left, Left);
    XKKK(Up, Up);
    XKKK(Right, Right);
    XKKK(Down, Down);
    XKKK(comma, Comma);
    XKKK(minus, Minus);
    XKKK(period, Dot);
    XKKK(slash, Slash);
    XKKK(equal, Equal);
    XKKK(Delete, Del);
    XKKK(Insert, Ins);
    XKKK(Home, Home);
    XKKK(End, End);
    XKKK(asterisk, KeypadAsterisk);
    XKKK(0, 0);
    XKKK(1, 1);
    XKKK(2, 2);
    XKKK(3, 3);
    XKKK(4, 4);
    XKKK(5, 5);
    XKKK(6, 6);
    XKKK(7, 7);
    XKKK(8, 8);
    XKKK(9, 9);
    XKKK(a, A);
    XKKK(b, B);
    XKKK(c, C);
    XKKK(d, D);
    XKKK(e, E);
    XKKK(f, F);
    XKKK(g, G);
    XKKK(h, H);
    XKKK(i, I);
    XKKK(j, J);
    XKKK(k, K);
    XKKK(l, L);
    XKKK(m, M);
    XKKK(n, N);
    XKKK(o, O);
    XKKK(p, P);
    XKKK(q, Q);
    XKKK(r, R);
    XKKK(s, S);
    XKKK(t, T);
    XKKK(u, U);
    XKKK(v, V);
    XKKK(w, W);
    XKKK(x, X);
    XKKK(y, Y);
    XKKK(z, Z);
	XKKK(KP_Add, padPlus);
	XKKK(KP_Subtract, padMinus);
    XKKK(F1, F1);
    XKKK(F2, F2);
    XKKK(F3, F3);
    XKKK(F4, F4);
    XKKK(F5, F5);
    XKKK(F6, F6);
    XKKK(F7, F7);
    XKKK(F8, F8);
    XKKK(F9, F9);
    XKKK(F10, F10);
    XKKK(F11, F11);
    XKKK(F12, F12);
    XKKK(Alt_L, LeftAlt);
    XKKK(Control_L, LeftCtrl);
    XKKK(Meta_L, LeftMeta);
    XKKK(Super_L, LeftSuper);
    XKKK(Shift_L, LeftShift);
    XKKK(Alt_R, RightAlt);
    XKKK(Control_R, RightCtrl);
    XKKK(Meta_R, RightMeta);
    XKKK(Super_R, RightSuper);
    XKKK(Shift_R, RightShift);
    XKKK(Caps_Lock, Capslock);
#undef XKKK
  }
  L("Unknown keysym 0x%04x", keysym);
  return Keys::KeyUnknown;
}

bool X11Keys::process(const XEvent &event) {
  bool pressed = false;
  switch (event.type) {
    case KeyPress:
      pressed = true;
    case KeyRelease:
      return evt_key(xkeyevent_to_keytype(&event.xkey), pressed, event.xkey.time);
  }
  return false;
}

} // namespace sys
} // namespace kapusha
