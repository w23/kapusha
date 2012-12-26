#include <kapusha/core/IViewport.h>
#include <kapusha/sys/rpi/RPi.h>

namespace fsquad {
  kapusha::IViewport *makeViewport();
}

namespace flyby {
  kapusha::IViewport *makeViewport();
}

int main(int argc, char *argv[])
{
  return kapusha::RunRaspberryRun(flyby::makeViewport());
}
