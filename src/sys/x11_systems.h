#pragma once

#include <stdio.h>
#include <errno.h>
#include <X11/Xlib.h>
#include <GL/glx.h>

#include "kapusha/app/systems.h"
#include "kapusha/app/input.h"
#include "kapusha/render/context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  KPsystems_i iface;

  Display *x11_display;
  Window x11_window;

  KPwindow_i *window;
  KPvec2i size;

  KPevent_handler_t key_handler;
  KPevent_handler_t pointer_handler;

  KPinput_key_t key;
  KPinput_pointer_t pointer;

} KPsystems_x11_t;

typedef struct {
  KPglcontext_i iface;
  Display *display;
  GLXFBConfig fbconfig;
  GLXDrawable drawable;
  GLXContext context;
} KPglcontext_glx_t;

void KP_systems_x11_ctor(KPsystems_x11_t *x11);
void KP_systems_x11_dtor(KPsystems_x11_t *x11);

#ifdef __cplusplus
} // extern "C"
#endif
