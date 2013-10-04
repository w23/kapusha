#pragma once
#include "windows_inc.h"

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