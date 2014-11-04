#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <GL/glx.h>

#include "x11.h"

#define KP__SYS "X11"

typedef struct KP__x11_output_t { KPoutput_video_t parent;
  int screenx, screeny;
} *KP__x11_output_o;

typedef struct KP__x11_window_t { KP_O;
  void *user_data;
  KPwindow_painter_create_f painter_create_func;
  KPwindow_painter_configure_f painter_configure_func;
  KPwindow_painter_f painter_func;
  GLXFBConfig *glxconfigs;
  int nglxconfigs;
  XVisualInfo *vinfo;
  Window window;
  GLXDrawable drawable;
  GLXContext context;
  KP__x11_output_o output;

  KPs32_atomic update;
  KPmutex_t mutex;
  int width, height;
  KPthread_t painter_thread;
} KP__x11_window_t, *KP__x11_window_o;

enum {
  KP__X11WindowUpdateNone,
  KP__X11WindowUpdateConfig,
  KP__X11WindowUpdateStop
} KP__X11WindowUpdate;

////////////////////////////////////////////////////////////////////////////////

static struct {
  Display *display;
  int xrr_event_base;
  int xrr_error_base;

  struct {
#define MAX_OUTPUTS 8
    KP__x11_output_o outputs[MAX_OUTPUTS];
  } outputs;
} g = {0};

/*
static void kp__OutputAdd(KP__x11_output_o *output) {
}
*/

KPiterable_o kpOutputsSelect(int *selectors) {
  return 0;
}

/* FIXME */
static KP__x11_window_o window = 0;

void kp__X11Init() {
  KP_ASSERT(g.display == 0);
  XInitThreads();
  g.display = XOpenDisplay(0);
  KP_ASSERT(g.display != 0);

  kpMemset(&g.outputs, 0, sizeof(g.outputs));

  const Bool xrr = XRRQueryExtension(g.display, &g.xrr_event_base, &g.xrr_error_base);
  KP_ASSERT(xrr == True);

  /* int oindex = 0; */
  const int nscreens = XScreenCount(g.display);
  for (int i = 0; i < nscreens; ++i) {
    KP__L("screen %d/%d", i+1, nscreens);
    Screen *screen = XScreenOfDisplay(g.display, i);
    KP__L("  size: %dx%dpx, %dx%dmm",
      screen->width, screen->height,
      screen->mwidth, screen->mheight);

    /* XSelectInput(g.display, screen->root, StructureNotifyMask); */

    XRRScreenResources *resources = XRRGetScreenResources(g.display, screen->root);
    for (int j = 0; j < resources->noutput; ++j) {
      XRROutputInfo *info = XRRGetOutputInfo(g.display, resources, resources->outputs[j]);
      KP__L("  output %d/%d: %s", j+1, resources->noutput, info->name);
      if (info->connection == RR_Connected && info->crtc != None) {
        KP__L("    size %dx%dmm", info->mm_width, info->mm_height);
        XRRCrtcInfo *crtc = XRRGetCrtcInfo(g.display, resources, info->crtc);
        KP__L("    %d,%d %dx%d", crtc->x, crtc->y, crtc->width, crtc->height);
        XRRFreeCrtcInfo(crtc);
      } else KP__L("   DISCONNECTED");
      XRRFreeOutputInfo(info);
    }
    XRRFreeScreenResources(resources);
  }
}

void kp__X11Run() {
  XEvent event;
  for (;;) {
    XNextEvent(g.display, &event);
    switch (event.type) {
      case ConfigureNotify:
/*        window->size(kpVec2i(
            event.xconfigure.width, event.xconfigure.height);*/
        break;

      case KeyPress:
      case KeyRelease:
/*        handle_key(&event.xkey, &x11->keys, &x11->key_handler);*/
        break;

      case ButtonPress:
      case ButtonRelease:
/*        handle_pointer_button(&event.xbutton, &x11->pointer, &x11->pointer_handler);*/
        break;

      case MotionNotify:
/*        handle_pointer_motion(&event.xmotion, &x11->pointer, &x11->pointer_handler);*/
        break;

      default:
        KP__L("Unexpected XEvent %d", event.type);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

static void kp__X11WindowDtor(void *obj) {
  KP_THIS(KP__x11_window_t, obj);
  kpMutexLock(&this->mutex);
  this->update = KP__X11WindowUpdateStop;
  kpMutexLock(&this->mutex);
  kpThreadJoin(this->painter_thread);

  if (this->context != 0) glXDestroyContext(g.display, this->context);
  if (this->drawable != 0) glXDestroyWindow(g.display, this->drawable);
  if (this->window != 0) XDestroyWindow(g.display, this->window);
  if (this->vinfo != 0) XFree(this->vinfo);
  if (this->glxconfigs != 0) XFree(this->glxconfigs);
  kpRelease(this->output);
  kpMutexDestroy(&this->mutex);
}

static KP__x11_window_o kp__X11WindowCreate(const KPwindow_params_t *params) {
  KP__x11_window_o this = KP_NEW(KP__x11_window_t, 0);
  this->O.dtor = kp__X11WindowDtor;
  this->user_data = params->user_data;
  this->painter_create_func = params->painter_create_func;
  this->painter_configure_func = params->painter_configure_func;
  this->painter_func = params->painter_func;
  this->glxconfigs = 0;
  this->nglxconfigs = 0;
  this->vinfo = 0;
  this->window = 0;
  this->drawable = 0;
  this->context = 0;

  int attrs[] = {
    GLX_X_RENDERABLE, True,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_CONFIG_CAVEAT, GLX_NONE,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_ALPHA_SIZE, 8,
    GLX_DEPTH_SIZE, 24,
    GLX_DOUBLEBUFFER, True,
/*    GLX_SAMPLE_BUFFERS, 1,
    GLX_SAMPLES, 4,*/
    0
  };
  this->glxconfigs = glXChooseFBConfig(g.display, 0/*screen*/, attrs, &this->nglxconfigs);
  KP_ASSERT(this->glxconfigs != 0);
  KP_ASSERT(this->nglxconfigs > 0);

  this->vinfo = glXGetVisualFromFBConfig(g.display, this->glxconfigs[0]);
  KP_ASSERT(this->vinfo != 0);

  this->context = glXCreateNewContext(g.display, this->glxconfigs[0],
    GLX_RGBA_TYPE, /*share_list*/ 0, True);
  KP_ASSERT(this->context != 0);

  XSetWindowAttributes winattrs;
  winattrs.event_mask =
    ExposureMask | VisibilityChangeMask | StructureNotifyMask |
    KeyPressMask | PointerMotionMask;
  winattrs.border_pixel = 0;
  winattrs.bit_gravity = StaticGravity;
  winattrs.colormap = XCreateColormap(g.display,
    RootWindow(g.display, this->vinfo->screen),
    this->vinfo->visual, AllocNone);

  this->width = params->width;
  this->height = params->height;

  this->window = XCreateWindow(g.display, RootWindow(g.display, this->vinfo->screen),
    0, 0, this->width, this->height, 0,
    this->vinfo->depth, InputOutput, this->vinfo->visual,
    CWBorderPixel | CWBitGravity | CWEventMask | CWColormap, &winattrs);

  XStoreName(g.display, this->window, params->title);
  XMapWindow(g.display, this->window);

  this->drawable = glXCreateWindow(g.display, this->glxconfigs[0], this->window, 0);
  KP_ASSERT(this->drawable != 0);

  XSelectInput(g.display, this->window,
    KeyPressMask | KeyReleaseMask | ButtonPressMask |
    ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);

  this->update = KP__X11WindowUpdateConfig;
  kpMutexInit(&this->mutex);
  return this;
}

static void *kp__X11WindowThreadFunc(void *user_data) {
  KP__x11_window_o w = (KP__x11_window_o)user_data;
  KPtime_ms time_prev, time_now;

  glXMakeContextCurrent(g.display, w->drawable, w->drawable, w->context);

  KP__L("[%p] window started painting", w);

  KPwindow_painter_create_t create;
  KPwindow_painter_configure_t config;
  KPwindow_painter_t paint;
  create.window = config.window = paint.window = w;
  create.user_data = config.user_data = paint.user_data = w->user_data;
  paint.time_delta = paint.time_delta_frame = 16000000;/*FIXME w->output->parent.frame_delta;*/

  w->painter_create_func(&create);

  // balance the tparams.user_data = kpRetain(window);
  kpRelease(w);

  time_now = time_prev = kpSysTimeNs();
  for (;;) {
    kpAtomicSynchronize();
    const int update = kpS32AtomicLoad(&w->update);
    if (update == KP__X11WindowUpdateConfig) {
      kpMutexLock(&w->mutex);
      config.width = w->width;
      config.height = w->height;
      w->update = KP__X11WindowUpdateNone;
      kpMutexUnlock(&w->mutex);
      w->painter_configure_func(&config);
    } else if (update == KP__X11WindowUpdateStop) break;

    paint.pts_estimate = time_now + paint.time_delta_frame;
    w->painter_func(&paint);

    glXSwapBuffers(g.display, w->drawable);

    time_prev = time_now;
    time_now = kpSysTimeNs();
    paint.time_delta = time_now - time_prev;
  }

  glXMakeContextCurrent(g.display, w->drawable, w->drawable, 0);
  KP__L("[%p] window stopped painting", w);
  return 0;
}

#if 0
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
  KP__L("Unknown keysym 0x%04x", keysym);
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

KPwindow_o kpWindowCreate(const KPwindow_params_t *params) {
  KP_ASSERT(window == 0);
  window = kp__X11WindowCreate(params);

  KPthread_params_t tparams;
  tparams.user_data = kpRetain(window);
  tparams.name = "window";
  tparams.thread_func = kp__X11WindowThreadFunc;
  window->painter_thread = kpThreadSpawn(&tparams);

  return window;
}

