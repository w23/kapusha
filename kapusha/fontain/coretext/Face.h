#pragma once
#include <CoreText/CoreText.h>
#include <CoreGraphics/CoreGraphics.h>
#include <kapusha/math.h>
#include <kapusha/utils/Atlas.h>
#include "../IFace.h"

namespace kapusha {
namespace fontain {
namespace coretext {
  class Face : public IFace {
  public:
    virtual ~Face();
    virtual String *createString(const char *string, int length = -1) const;
  //protected:
    Face(const char *font_name, vec2i size);
  private:
    struct Glyph {
      vec2i origin;
      rect2i atlas;
    };
    CFAttributedStringRef makeAttributedString(const char *string,
                                               int length) const;
    Glyph getGlyph(CGGlyph glyph) const;

    CTFontRef font_;
    CGFontRef cgFont_;
    CGContextRef atlasContext_;
    SAtlas atlas_;
    MapOf<Glyph> grapheMap_;
  };
} // namespace coretext
} // namespace fontain
} // namespace kapusha