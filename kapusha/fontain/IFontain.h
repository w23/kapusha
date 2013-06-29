#pragma once
#include <kapusha/math.h>
#include "IFace.h"

namespace kapusha {
  class IFontain {
  public:
    virtual fontain::IFace *getFace(const char *name, vec2i size) = 0;
  };
} // namespace kapusha
