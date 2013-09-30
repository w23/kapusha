#pragma once
#include "../math.h"

namespace kapusha {
class IViewport;
class IViewportFactory;

//! Kapusha-powered application startup settings
struct Application {
  const char *title;
  vec2i prefer_resolution;
  const IViewportFactory *viewport_factory;
};

extern Application the_application;

} // namespace kapusha