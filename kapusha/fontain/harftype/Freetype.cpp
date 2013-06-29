#include "Freetype.h"
#include "Face.h"

namespace kapusha {
namespace fontain {
namespace harftype {
Freetype::Freetype() {
  FT_Error error = FT_Init_FreeType(&library_);
  if (error != 0) L("Error initializing freetype: %d", error);
}
Freetype::~Freetype() {
  FT_Done_FreeType(library_);
}

IFace *Freetype::getFace(const char *name, vec2i size) {
  return Face::create(library_, name, size);
}

} // namespace harftype
} // namespace fontain
} // namespace kapusha
