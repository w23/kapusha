#pragma once
#include "../../core/IViewport.h"

namespace kapusha {
  int runGlut(int argc, char *argv[], vec2i size, IViewport *viewport);
}