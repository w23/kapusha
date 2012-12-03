#pragma once
#include <Windows.h>

namespace kapusha {
  class IViewport;
  int RunWindow(HINSTANCE hInst, IViewport *viewport,
                int width, int height, bool fullscreen);
}