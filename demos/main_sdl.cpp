#include <kapusha/core.h>
#include <kapusha/sys/sdl/KPSDL.h>

kapusha::IViewport *makeViewport();

int main(int argc, char* argv[])
{
  KP_LOG_OPEN(0);
  return kapusha::KPSDL(makeViewport(), 800, 600);
}
