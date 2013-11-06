#pragma once
#include "../core.h"

namespace kapusha {
  //! Basic input state with timestamp
  class Input {
  public:
    inline u32 time_stamp() const { return timestamp_; }
    inline u32 time_delta() const { return timestamp_delta_; }

  protected:
    inline void evt_accumulate(u32 time) { timestamp_delta_ += time - timestamp_, timestamp_ = time; }
    inline void evt_update(u32 time) { timestamp_delta_ = 0, evt_accumulate(time); }
    inline void evt_reset_delta() { timestamp_delta_ = 0; }
  private:
    u32 timestamp_;
    u32 timestamp_delta_;
  }; // class Input
} // namespace kapusha
