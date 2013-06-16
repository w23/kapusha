#include <stdio.h>
#include <errno.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <xcb/xcb.h>
#include <GL/glx.h>
#ifdef None
#undef None // this is ridiculous
#endif
#ifdef AnyButton
#undef AnyButton // this is ridiculous too
#endif
#include <kapusha/core.h>
#include <kapusha/viewport.h>
#include <kapusha/render/Context.h>
