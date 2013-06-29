#pragma once
#include <hb.h>
#include <hb-ft.h>
#include <kapusha/core.h>
#include <kapusha/utils/Atlas.h>
#include "Freetype.h"

namespace kapusha {
namespace fontain {
namespace harftype {

class Freetype;
class Face : public IFace {
public:
  virtual ~Face();
  virtual String *createString(Context *ctx, const char *string, int length = -1) const;
protected:
  friend class Freetype;
  friend class String;
  static Face *create(FT_Library library, const char *font_name, vec2i size);
  struct Glyph {
    vec2i origin;
    rect2i atlas;
  };
  Glyph getGlyphForCodepoint(u32 codepoint) const;
private:
  inline Face(vec2i atlas_size) : font_(NULL),
    atlas_(Surface::Meta(atlas_size, Surface::Meta::R8)) {}
  FT_Face face_;
  hb_font_t *font_;
  Atlas atlas_;
  MapOf<Glyph> grapheMap_;
}; // class Face
} // namespace harftype
} // namespace fontain
} // namespace kapusha
