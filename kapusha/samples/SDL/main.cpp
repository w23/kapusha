#include <iostream>
#include <kapusha/core/core.h>
#include <kapusha/sys/sdl/KPSDL.h>

namespace kapusha {
  class IViewport;
}

namespace flyby {
  kapusha::IViewport *makeViewport();
}

namespace fsquad {
  kapusha::IViewport *makeViewport();
}

int main(int argc, char* argv[])
{
  KP_LOG_OPEN(0);
  return kapusha::KPSDL(fsquad::makeViewport(), 800, 600);
}
