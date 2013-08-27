#include <stdio.h>
#include <kapusha/core/log.h>
#include <kapusha/app.h>
#include "KPSDL.h"

using namespace kapusha;

void log::sys_write(const char *message) {
  fprintf(stderr, "[KP] %s\n", message);
}

int main(int argc, char* argv[])
{
  return KPSDL(the_application.createViewport(),
    the_application.prefer_resolution.x, the_application.prefer_resolution.y, false);
}
