#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../IFontain.h"

namespace kapusha {
namespace fontain {
namespace harftype {
class Freetype : public IFontain {
public:
  Freetype();
  ~Freetype();
  virtual SFace getFace(const char *name, vec2i size);
private:
  FT_Library library_;
}; // class Freetype
} // namespace harftype
} // namespace fontain
} // namespace kapusha
