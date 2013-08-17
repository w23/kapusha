#pragma once
#include "../math.h"

namespace kapusha {
class IViewport;

struct Application {
  vec2i prefer_resolution;
  IViewport *createViewport() const;
};

extern Application the_application;

} // namespace kapusha