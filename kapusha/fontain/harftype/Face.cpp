#include "String.h"
#include "Face.h"

namespace kapusha {
namespace fontain {
namespace harftype {

Face::~Face() {
  if (font_) hb_font_destroy(font_);
  FT_Done_Face(face_);
}

IString *Face::createEmptyString() const {
  return new String(const_cast<Face*>(this));
}

SFace Face::create(FT_Library library, const char *font_name, vec2i size) {
  Face *face = new Face;
  SFace ret(face);

  FT_Error error;
  error = FT_New_Face(library, font_name, 0, &face->face_);
  if (error != 0) {
    L("Cannot create face \"%s\": %d", font_name, error);
    return SFace();
  }
  KP_ASSERT(error == 0);

  L("Face \"%s\" created. Some info:", font_name);
  L("num_glyphs = %d", face->face_->num_glyphs);
  L("face_flags = %08x", face->face_->face_flags);

  error = FT_Set_Pixel_Sizes(face->face_, size.x, size.y);
  if (error != 0) {
    L("Cannot set face \"%s\" to size (%d, %d): %d", font_name, size.x, size.y, error);
    return SFace();
  }
  KP_ASSERT(error == 0);

  face->font_ = hb_ft_font_create(face->face_, NULL);
  return ret;
}

const Face::Glyph *Face::getGlyphForCodepoint(unsigned codepoint) {
  FT_Error error = FT_Load_Glyph(face_, codepoint, FT_LOAD_RENDER);
  if (error != 0) {
    L("FT_Load_GLyph(%d) = %d", codepoint, error);
    return nullptr;
  }

  FT_GlyphSlotRec *slot = face_->glyph;
  const vec2i origin = vec2i(slot->bitmap_left, slot->bitmap_top);
  const vec2i size = vec2i(slot->bitmap.width, -slot->bitmap.rows);
  //L("G O:(%d, %d), S(%d, %d) P%d", origin.x, origin.y, size.x, size.y,
  //  slot->bitmap.pitch);
  currentGlyph_.box.clear();
  currentGlyph_.box.extendToContain(origin);
  currentGlyph_.box.extendToContain(origin + size);
  currentGlyph_.bitmap = &face_->glyph->bitmap;
  return &currentGlyph_;
}

} // namespace harftype
} // namespace fontain
} // namespace kapusha
