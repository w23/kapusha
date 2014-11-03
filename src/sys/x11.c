#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <GL/glx.h>

#include "x11.h"

typedef struct {
  GLXDrawable dest;
  GLXContext context;
} KP__glx_context_t;

void KP__GLXCreate(KP__glx_context_t);
void KP__GLXDestroy(KP__glx_context_t);

typedef struct { KP_O;
  KPwindow_params_t params;

  GLXFBConfig *glxconfigs;
  int nglxconfigs;
  XVisualInfo *vinfo;
  Window window;

  KP__glx_context_t context;
} *KP__x11_window_o;

typedef struct KP__x11_output_t { KPoutput_video_t parent;
  int screenx, screeny;
} *KP__x11_output_o;

////////////////////////////////////////////////////////////////////////////////

static Display *display = 0;
static int xrr_event_base;
static int xrr_error_base;

void kp__X11Init() {
  KP_ASSERT(display == 0);
  display = XOpenDisplay(0);
  KP_ASSERT(display != 0);

  const Bool xrr = XRRQueryExtension(display, &xrr_event_base, &xrr_error_base);
  KP_ASSERT(xrr == True);

  const int nscreens = XScreenCount(display);
  for (int i = 0; i < nscreens; ++i) {
    KP_L("screen %d/%d", i+1, nscreens);
    Screen *screen = XScreenOfDisplay(display, i);
    KP_L("  size: %dx%dpx, %dx%dmm",
      screen->width, screen->height,
      screen->mwidth, screen->mheight);

    XRRScreenResources *resources = XRRGetScreenResources(display, screen->root);
    for (int j = 0; j < resources->noutput; ++j) {
      XRROutputInfo *info = XRRGetOutputInfo(display, resources, resources->outputs[j]);
      KP_L("  output %d/%d: %s", j+1, resources->noutput, info->name);
      if (info->connection == RR_Connected) {
        KP_L("    size %dx%dmm", info->mm_width, info->mm_height);
        XRRCrtcInfo *crtc = XRRGetCrtcInfo(display, resources, info->crtc);
        KP_L("    %d,%d %dx%d", crtc->x, crtc->y, crtc->width, crtc->height);
        XRRFreeCrtcInfo(crtc);
      } else KP_L("   DISCONNECTED");
      XRRFreeOutputInfo(info);
    }
    XRRFreeScreenResources(resources);
  }
}

void kp__X11Run() {
}
#if 0
  XEvent event;

  /* TODO NOTE fd-runloop integration: XInternalConntectionNumbers and friends */
  for (;;) {
    while (XPending(display)) {
      XNextEvent(display, &event);
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

  }

}

static void kp__GLXContextCreate(KP__glx_context_t *);

static void kp__X11WindowOpen(KP__x11_window_o *);
static void kp__X11WindowDtor(KP__window_x11_o w);
static void kp__X11WindowThreadFunc(KPthread_context_t *ctx);

KPwindow_o kpWindowCreate(
  const KPwindow_params_t *params,
  void (*window_event)(const KPwindows_event_t *event))
{
  KP__x11_window_o window = KP_NEW(*KP__x11_window_o, 0);
  window.dtor = kp__X11WindowDtor;
  kpMemcpy(window->params, params, sizeof(*params));

  kp__GLXContextCreate(&window->context);
  kp__X11WindowOpen(&window);

  KPthread_params_t tparams;
  tparams.user_data = kpRetain(window);
  tparams.name = "window";
  tparams.thread_func = kp__X11WindowThreadFunc;
  kpThreadSpawn(&tparams);

  return window;
}

static void kp__X11WindowDtor(KP__window_x11_o w) {
  KP_FAIL("Window dtor not implemented");

  if (w->window != NULL) XDestroyWindow(display, w->window);
  if (xvis != NULL) XFree(xvis);
  XFree(glxconfigs);
  return 0;
}

static void kp__GLXContextCreate(KP__glx_context_t *context) {
  int attrs[] = {
    /* ? GLX_X_RENDERABLE, True, */
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_CONFIG_CAVEAT, GLX_NONE,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_DEPTH_SIZE, 24,
    GLX_DOUBLEBUFFER, True,
    GLX_SAMPLE_BUFFERS, 1,
    GLX_SAMPLES, 4,
    0
  };
  context->configs = glXChooseFBConfig(display, 0, attrs, &context->nconfigs);
  KP_ASSERT(context->configs != 0 || context->nconfigs > 0);

  context->visualinfo = glXGetVisualFromFBConfig(display, context->configs[0]);
  KP_ASSERT(context->visualinfo != 0);

  context->context = glXCreateNewContext(display, context->configs[0],
    GLX_RGBA_TYPE, /*share_list*/ 0, True);
  KP_ASSERT(context->context != 0);
}

static void kp__X11WindowOpen(KP__x11_window_o w) {
  XSetWindowAttributes winattrs;
  winattrs.event_mask =
    ExposureMask | VisibilityChangeMask | StructureNotifyMask |
    KeyPressMask | PointerMotionMask;
  winattrs.border_pixel = 0;
  winattrs.bit_gravity = StaticGravity;
  winattrs.colormap = XCreateColormap(display,
    RootWindow(display, w->context.visualinfo->screen),
    w->context.visualinfo>visual, AllocNone);

  w->window = XCreateWindow(display, DefaultRootWindow(display),
    0, 0, w->params.width, w->params.height, 0,
    w->context.visualinfo->depth, InputOutput, w->context.visualinfo->visual,
    CWBorderPixel | CWBitGravity | CWEventMask | CWColormap, &winattrs);

  XStoreName(display, w->window, w->params.name);

  XMapWindow(display, w->window);

  context->context.window = glXCreateWindow(display, w->context.configs[0], w->window, 0);
  KP_ASSERT(context->context.window != 0);

  XSelectInput(display, w->window,
    KeyPressMask | KeyReleaseMask | ButtonPressMask |
    ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
}

static void kp__WindowX11ThreadFunc(void *user_data) {
  KP__x11_window_o w = (KP__x11_window_o)user_data;
  KPtime_ms time_prev, time_now, time_start;

  glXMakeContextCurrent(display, w->context.dest, w->context.dest, w->context.context);

  time_prev = time_start = KP_now();
  for (;;) {
    time_now = KP_now();
    x11->window->draw(time_prev, time_prev + 16);
    glXSwapBuffers(display, w->context.dest);
    time_prev = now;
  }

  KP_glcontext_make_current(NULL);
  KP_glcontext_glx_dtor(&ctx);

  x11->window->dtor(x11->window);
}


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
#endif
