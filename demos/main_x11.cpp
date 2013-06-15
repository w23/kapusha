#include <kapusha/core.h>
#include <kapusha/sys/x11/x11.h>

using namespace kapusha;

IViewport *makeViewport();

int main(int argc, char* argv[])
{
  KP_LOG_OPEN(0);
  return X11Run(makeViewport(), vec2i(800, 600), false);
}
