#include "x11_systems.h"

int main(int argc, char *argv[]) {
  KPsystems_x11_t x11;
  KP_systems_x11_ctor(&x11);
  int result = KP_main(argc, argv, &x11);
  KP_systems_x11_dtor(&x11);
  return result;
}
