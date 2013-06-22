#pragma once
#include <kapusha/core.h>
#include <kapusha/math.h>

namespace kapusha {
namespace fontain {
  class IString;
  class IFace : public Shareable {
  public:
    virtual IString *createEmptyString() const = 0;
  };
  typedef shared<IFace> SFace;
} // namespace fontain
} // namespace kapusha
