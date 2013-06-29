#pragma once

namespace kapusha {
  class Context;
namespace fontain {
  class String;
  class IFace {
  public:
    virtual ~IFace() {}
    virtual String *createString(Context *ctx,const char *string, int length = -1) const = 0;
  }; // class IFace
} // namespace fontain
} // namespace kapusha
