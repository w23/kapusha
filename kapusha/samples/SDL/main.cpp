#include <kapusha/sys/SDL/KPSDL.h>

namespace kapusha {
  class IViewport;
}

namespace flyby {
  kapusha::IViewport *makeViewport();
}

int main(int argc, char* argv[])
{
  return kapusha::KPSDL(flyby::makeViewport(), 800, 600);
}
