#pragma once
#include "IFace.h"

namespace kapusha {
  class IFontain {
  public:
    virtual fontain::SFace getFace(const char *name, vec2i size) = 0;
  };
} // namespace kapusha
