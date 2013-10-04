#pragma once
#include "windows_inc.h"

namespace kapusha {
  class IViewportFactory;
  int RunWindow(HINSTANCE hInst, const IViewportFactory *factory);
}
