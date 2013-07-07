#include "String.h"

namespace kapusha {
namespace fontain {

  String::String(Atlas *atlas, u32 nglyphs)
  : atlas_(atlas), glyphs_(nglyphs) {
    KP_ASSERT(atlas_.get());
    box_.clear();
  }

  String::~String() {
  }
} // namespace fontain
} // namespace kapusha
