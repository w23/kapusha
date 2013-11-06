#include "X11Keys.h"

namespace kapusha {
namespace sys {


static Keys::Types xkeyevent_to_keytype(const XKeyEvent *event) {
  switch (XLookupKeysym(const_cast<XKeyEvent*>(event), 0)) {
#define XKKK(xk,kk) case XK_##xk: return Keys::Key##kk
    XKKK(BackSpace, Backspace);
    XKKK(Tab,Tab);
    XKKK(Return, Enter);
    XKKK(space, Space);
    XKKK(Escape, Esc);
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
#undef XKKK
  }
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
