#include "kapusha/sys.h"
#include "kapusha/app/event.h"
#include "x11_systems.h"

static int KP_systems_x11_run(void *param, const KPpreferences_t *preferences);
static void KP_systems_x11_set_handler(void *param, int event_class,
    void *user_param, KPevent_handler_f func);
static void KP_systems_x11_exit(void *param, int code);

void KP_systems_x11_ctor(KPsystems_x11_t *x11) {
  x11->x11_display = XOpenDisplay(NULL);
  if (x11->x11_display == NULL) {
    KP_L("Cannot connect to X server");
    kpSysExit(EFAULT);
  }

  x11->iface.param = x11;
  x11->iface.run = KP_systems_x11_run;
  x11->iface.set_handler = KP_systems_x11_set_handler;
  x11->iface.exit = KP_systems_x11_exit;

  x11->x11_window = 0;
  x11->window = NULL;
  x11->size = KP_vec2i(0, 0);

  x11->key_handler.func = NULL;
  x11->pointer_handler.func = NULL;
}

void KP_systems_x11_dtor(KPsystems_x11_t *x11) {
  XCloseDisplay(x11->x11_display);
}

void kpGLContextMakeCurrent(KPglcontext_i *ctx) {
  KP_RASSERT(True == glXMakeCurrent(ctx->display, ctx->drawable, ctx->context));
}

static void KP_glcontext_glx_dtor(KPglcontext_glx_t *ctx) {
  glXDestroyContext(ctx->display, ctx->context);
}

static void KP_glcontext_glx_ctor(KPglcontext_glx_t *ctx,
    Display *display, GLXFBConfig fbconfig, GLXDrawable drawable) {
  ctx->iface.param = ctx;
  ctx->iface.dtor = (KPglcontext_dtor_f)KP_glcontext_glx_dtor;
  ctx->display = display;
  ctx->fbconfig = fbconfig;
  ctx->drawable = drawable;
  ctx->context = glXCreateNewContext(display, fbconfig, GLX_RGBA_TYPE, NULL, True);
  KP_ASSERT(ctx->context != NULL);
}

static KPKey xkeyevent_to_keytype(XKeyEvent *event) {
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

static inline void handle_key(XKeyEvent *xkey,
    KPinput_key_t *keys, KPevent_handler_t *handler) {
  KPevent_key_t event;
  event.key = xkeyevent_to_keytype(xkey);
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

static inline void handle_pointer_button(XButtonEvent *xbutton,
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

static inline void handle_pointer_motion(XMotionEvent *xmotion,
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

static int KP_systems_x11_run(void *param, const KPpreferences_t *prefs) {
  KPsystems_x11_t *x11 = (KPsystems_x11_t*)param;
  int ret = 0;
  XVisualInfo *xvis = NULL;
  XSetWindowAttributes winattrs;
  int glxcount;
  KPglcontext_glx_t ctx;
  XEvent event;
  KPtime_ms time_prev, time_now, time_start;
  int glxattribs[] = {
    GLX_X_RENDERABLE, True,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_CONFIG_CAVEAT, GLX_NONE,
    GLX_BUFFER_SIZE, 24, ///< \todo replace with individual colors
    /// \todo GLX_RED_SIZE, GLX_GREEN_SIZE, GLX_BLUE_SIZE,
    GLX_DEPTH_SIZE, 24,
    GLX_DOUBLEBUFFER, True,
    GLX_SAMPLE_BUFFERS, 1,
    GLX_SAMPLES, 4,
    0
  };
  GLXFBConfig *glxconfigs = glXChooseFBConfig(x11->x11_display, 0, glxattribs, &glxcount);

  if (glxconfigs == NULL || glxcount < 1) {
    KP_L("Cannot find appropriate glx framebuffer configuration");
    return EINVAL;
  }

  xvis = glXGetVisualFromFBConfig(x11->x11_display, glxconfigs[0]);
  if (xvis == NULL) {
    KP_L("Cannot find appropriate x11 visual");
    ret = EINVAL;
    goto exit;
  }

  winattrs.event_mask =
    ExposureMask | VisibilityChangeMask | KeyPressMask |
    PointerMotionMask | StructureNotifyMask;
  winattrs.border_pixel = 0;
  winattrs.bit_gravity = StaticGravity;
  winattrs.colormap = XCreateColormap(x11->x11_display,
    RootWindow(x11->x11_display, xvis->screen),
    xvis->visual, AllocNone);

  x11->x11_window = XCreateWindow(x11->x11_display, DefaultRootWindow(x11->x11_display),
    0, 0, 1280, 720, 0, xvis->depth, InputOutput, xvis->visual,
    CWBorderPixel | CWBitGravity | CWEventMask | CWColormap, &winattrs);
  XMapWindow(x11->x11_display, x11->x11_window);

  /* glXCreateWindow? */

  XSelectInput(x11->x11_display, x11->x11_window,
    KeyPressMask | KeyReleaseMask | ButtonPressMask |
    ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);

  KP_glcontext_glx_ctor(&ctx, x11->x11_display, glxconfigs[0], x11->x11_window);
  KP_glcontext_make_current(&ctx);

  x11->window = prefs->init_window(prefs->param, &x11.system);
  if (x11->window == NULL) {
    KP_L("No window");
    ret = EINVAL;
    goto exit:
  }

  /* TODO NOTE fd-runloop integration: XInternalConntectionNumbers and friends */

  time_prev = time_start = KP_now();
  for (;;) {
    while (XPending(x11->x11_display)) {
      XNextEvent(x11->x11_display, &event);
      switch (event.type) {
        case ConfigureNotify:
          x11->window->size(kpVec2i(
              event.xconfigure.width, event.xconfigure.height);
          break;

        case KeyPress:
        case KeyRelease:
          handle_key(&event.xkey, &x11->keys, &x11->key_handler);
          break;

        case ButtonPress:
        case ButtonRelease:
          handle_pointer_button(&event.xbutton, &x11->pointer, &x11->pointer_handler);
          break;

        case MotionNotify:
          handle_pointer_motion(&event.xmotion, &x11->pointer, &x11->pointer_handler);
          break;

        default:
          L("Unexpected XEvent %d", event.type);
      }
    }

    time_now = KP_now();
    x11->window->draw(time_prev, time_prev + 16); /* TODO estimate next vsync */
    glXSwapBuffers(x11->x11_display, x11->x11_window);
    time_prev = now;
  }

  KP_glcontext_make_current(NULL);
  KP_glcontext_glx_dtor(&ctx);

  x11->window->dtor(x11->window);

exit:
  if (x11->x11_window != NULL)
    XDestroyWindow(x11->x11_display, x11->x11_window);
  if (xvis != NULL)
    XFree(xvis);
  XFree(glxconfigs);
  return 0;
}

static void KP_systems_x11_set_handler(void *param, int event_class,
    void *user_param, KPevent_handler_f func) {
  /* TODO thread-safety */
  KPsystems_x11_t *x11 = (KPsystems_x11_t*)param;
  switch (event_class) {
    case KPEventClass_Keyboard:
      x11->key_handler.param = user_param;
      x11->key_handler.func = func;
      break;
    case KPEventClass_Pointer:
      x11->pointer_handler.param = user_param;
      x11->pointer_handler.func = func;
      break;
    default:
      KP_L("Invalid event class %d", event_class);
  }
}

static void KP_systems_x11_exit(int code) {
  /* TODO gracefully stop */
  exit(code);
}
