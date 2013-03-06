#include <iostream>
#include <kapusha/core/Core.h>
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

class StderrLog : public kapusha::Log::ISystemLog {
public:
  void write(const char* msg) {
    std::cerr << msg << std::endl;
  }
};

int main(int argc, char* argv[])
{
  KP_LOG_OPEN(0, new StderrLog);
  return kapusha::KPSDL(flyby::makeViewport(), 800, 600);
}
