#pragma once
#include <kapusha/viewport.h>

namespace kapusha {

  class IViewport;

  int KPSDL(IViewport* viewport, int width, int height, bool fullscreen = false);

} // namespace kapusha
