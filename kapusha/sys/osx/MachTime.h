#pragma once
#include <mach/mach_time.h>
#import <Foundation/Foundation.h>
#include <kapusha/core.h>

namespace kapusha {
  class MachTime {
  public:
    MachTime() : start_(mach_absolute_time()) { mach_timebase_info(&timebase_); }
    void reset() { start_ = mach_absolute_time(); }
    u64 now_ns() {
      uint64_t now = mach_absolute_time() - start_;
      return now * timebase_.numer / timebase_.denom;
    }
    u32 now_ms() { return static_cast<u32>(now_ns() / 1000000ULL); }
    static u32 sys_to_ms(NSTimeInterval nsti) {
      /// \warning this is inconsistent with now_* time
      /// \todo make it consisntent
      return nsti / 1000.f;
    }
  private:
    uint64_t start_;
    mach_timebase_info_data_t timebase_;
  }; // class MachTime
} // namespace kapusha