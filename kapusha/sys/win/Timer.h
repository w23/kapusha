#pragma once
#ifndef __GNUC__
#pragma comment(lib, "winmm.lib")
#endif
#include "windows_inc.h"
#include <mmsystem.h>

namespace kapusha {
  class Timer {
  public:
    inline Timer() {
      timeBeginPeriod(1);
      start_ = timeGetTime();
    }

    inline ~Timer() {
      timeEndPeriod(1);
    }

    inline u32 now() const {
      return timeGetTime() - start_;
    }

  private:
    u32 start_;
  };
} // namespace kapusha