#pragma once
#include <hb.h>
#include <hb-ft.h>
#include "Freetype.h"

namespace kapusha {
namespace fontain {
namespace harftype {

class Freetype;
class Face : public IFace {
public:
  inline Face() : font_(NULL) {}
  virtual ~Face();
  virtual IString *createEmptyString() const;
protected:
  friend class Freetype;
  friend class String;
  static SFace create(FT_Library library, const char *font_name, vec2i size);
  struct Glyph {
    rect2i box;
    const FT_Bitmap *bitmap;
  };
  const Glyph *getGlyphForCodepoint(unsigned codepoint);
  inline hb_font_t *hbFont() const { return font_; }
private:
  FT_Face face_;
  hb_font_t *font_;
  Glyph currentGlyph_; //!< \todo implement actual glyph caching
}; // class Face
} // namespace harftype
} // namespace fontain
} // namespace kapusha
