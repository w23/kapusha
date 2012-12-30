#include <iostream>
#include <kapusha/core/IViewport.h>
#include <kapusha/sys/rpi/RPi.h>

namespace fsquad {
  kapusha::IViewport *makeViewport();
}

namespace flyby {
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
  KP_LOG_OPEN("samples.log", 0);//new StderrLog);
  return kapusha::RunRaspberryRun(flyby::makeViewport());
}
