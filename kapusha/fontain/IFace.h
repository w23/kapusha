#pragma once

namespace kapusha {
  class Context;
namespace fontain {
  class String;
  class IFace {
  public:
    virtual ~IFace() {}
    virtual String *createString(const char *string, int length = -1) const = 0;
    
    struct Metrics {
      float ascent, descent, leading;
    };
    inline const Metrics &metrics() const { return metrics_; }
    inline float lineHeight() const {
      return metrics_.leading + metrics_.ascent + metrics_.descent;
    }
  protected:
    Metrics metrics_;
  }; // class IFace
} // namespace fontain
} // namespace kapusha
