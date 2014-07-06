#include "kapusha/simpleton.h"
#include "../sys/x11.h"

int main(int argc, char *argv[]) {
  int retval = 0;
  int glxattribs[] = {
    GLX_X_RENDERABLE, True,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_CONFIG_CAVEAT, GLX_NONE,
    GLX_BUFFER_SIZE, 24,
    GLX_DEPTH_SIZE, 24,
    GLX_DOUBLEBUFFER, True,
    GLX_SAMPLE_BUFFERS, 1,
    GLX_SAMPLES, 4,
    0
  };

  Display *display;
  XSetWindowAttributes winattrs;
  Window window;
  XVisualInfo *visinfo = NULL;
  GLXFBConfig *glxconfigs = NULL;
  int glxconfigs_count;
  GLXContext context;

  display = XOpenDisplay(NULL);
  if (display == NULL) {
    KP_L("Cannot connect to X server");
    return EFAULT;
  }

  glxconfigs = glXChooseFBConfig(display, 0, glxattribs, &glxconfigs_count);

  if (glxconfigs == NULL || glxcount < 1) {
    KP_L("Cannot find appropriate glx framebuffer configuration");
    retval = EFAULT;
    goto exit;
  }

  visinfo = glXGetVisualFromFBConfig(display, glxconfigs[0]);
  if (visinfo == NULL) {
    KP_L("Cannot find appropriate x11 visual");
    retval = EFAULT;
    goto exit;
  }

  winattrs.event_mask =
    ExposureMask | VisibilityChangeMask | KeyPressMask |
    PointerMotionMask | StructureNotifyMask;
  winattrs.border_pixel = 0;
  winattrs.bit_gravity = StaticGravity;
  winattrs.colormap = XCreateColormap(display,
    RootWindow(display, visinfo->screen),
    visinfo->visual, AllocNone);

  /* TODO real monitor w/h */

  window = XCreateWindow(display, DefaultRootWindow(display),
    0, 0, 1280, 720, 0, visinfo->depth, InputOutput, visinfo->visual,
    CWBorderPixel | CWBitGravity | CWEventMask | CWColormap, &winattrs);
  XMapWindow(display, window);

  { /* go fullscreen using wm */
    Atom wm_state = XInternAtom(display, "_NET_WM_STATE", False);
    Atom fullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = window;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1; /* _NET_WM_STATE_ADD */
    xev.xclient.data.l[1] = fullscreen;
    xev.xclient.data.l[2] = 0;
    XSendEvent(display, DefaultRootWindow(display), False,
      SubstructureRedirectMask | SubstructureNotifyMask, &xev);
  }

  /* glXCreateWindow? */

  XSelectInput(display, window,
    KeyPressMask | KeyReleaseMask | ButtonPressMask |
    ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);

  context = glXCreateNewContext(display, glxconfigs[0], GLX_RGBA_TYPE, NULL, True);

  glXMakeCurrent(display, window, context);

  KP_L("Window and context ready, Initializing application");
  simpleton_init(argc, argv);

  {
    XEvent event;
    KPtime_ms time_now, time_prev = KP_now();
    for (;;) {
      while (XPending(display)) {
        XNextEvent(display, &event);
        switch (event.type) {
          case ConfigureNotify:
            simpleton_Size(event.xconfigure.width, event.xconfigure.height);
            break;

          default:
            L("Unexpected XEvent %d", event.type);
        }
      }

      time_now = KP_now();
      simpleton_draw(time_prev, time_prev + 16); /* TODO estimate next vsync */
      glXSwapBuffers(display, window);
      time_prev = now;
    }
  }

exit:
  if (window != NULL) XDestroyWindow(display, window);
  if (visinfo != NULL) XFree(visinfo);
  if (glxconfigs != NULL) XFree(glxconfigs);
  XCloseDisplay(display);
  return 0;
}
