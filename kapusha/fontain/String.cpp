#include "String.h"

namespace kapusha {
namespace fontain {

  String::String(Sampler *atlas, u32 nglyphs) : atlas_(atlas)
    , nglyphs_(nglyphs) {
    KP_ASSERT(atlas);
    glyphs_ = new Glyph[nglyphs_];
  }

  String::~String() {
    delete[] glyphs_;
  }
} // namespace fontain
} // namespace kapusha
