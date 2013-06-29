#include "../String.h"
#include "Face.h"

namespace kapusha {
namespace fontain {
namespace harftype {

Face::~Face() {
  if (font_) hb_font_destroy(font_);
  FT_Done_Face(face_);
}

//! \todo get rid of const_cast<>s somehow

String *Face::createString(Context *ctx, const char *string, int length) const {
  if (length == -1) length = strlen(string);

  // setup harfbuzz buffer
  hb_buffer_t *buffer = hb_buffer_create();
  //hb_buffer_reset(buffer);
  hb_buffer_add_utf8(buffer, string, length, 0, length);

  // calculate glyphs
  hb_shape(font_, buffer, NULL, 0);

  // get glyphs
  u32 glyphs;
  hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(buffer, &glyphs);
  hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buffer, &glyphs);

  // construct a string and populate it with glyphs and metrics
  if (!atlas_.getSampler()) const_cast<Atlas*>(&atlas_)->commit(ctx);
  String *stret = new String(const_cast<Sampler*>(atlas_.getSampler()), glyphs);
  vec2i pen(0);
  for (u32 i = 0; i < glyphs; ++i) {
    Glyph fg = getGlyphForCodepoint(glyph_info[i].codepoint);
    //KP_ASSERT(fg.atlas.isValid());

    const vec2i offset(glyph_pos[i].x_offset/64, glyph_pos[i].y_offset/64);
    const vec2i advance(glyph_pos[i].x_advance/64, glyph_pos[i].y_advance/64);
    //L("H O(%d, %d) A(%d, %d)", offset.x, offset.y, advance.x, advance.y);
    String::Glyph &g = (*stret)[i];
    g.clusterBegin = glyph_info[i].cluster;
    g.clusterEnd = (i < (glyphs-1)) ? glyph_info[i+1].cluster : length;
    g.offset = pen + offset + fg.origin;
    g.rectInAtlas = fg.atlas;
    pen += advance;
  }

  hb_buffer_destroy(buffer);
  const_cast<Atlas*>(&atlas_)->commit(ctx);
  return stret;
}

Face *Face::create(FT_Library library, const char *font_name, vec2i size) {
  Face *face = new Face(vec2i(256)); //! \todo depend on size

  FT_Error error;
  error = FT_New_Face(library, font_name, 0, &face->face_);
  if (error != 0) {
    L("Cannot create face \"%s\": %d", font_name, error);
    return nullptr;
  }
  KP_ASSERT(error == 0);

  L("Face \"%s\" created. Some info:", font_name);
  L("num_glyphs = %d", face->face_->num_glyphs);
  L("face_flags = %08x", face->face_->face_flags);

  error = FT_Set_Pixel_Sizes(face->face_, size.x, size.y);
  if (error != 0) {
    L("Cannot set face \"%s\" to size (%d, %d): %d", font_name, size.x, size.y, error);
    return nullptr;
  }
  KP_ASSERT(error == 0);

  face->font_ = hb_ft_font_create(face->face_, NULL);
  return face;
}

Face::Glyph Face::getGlyphForCodepoint(u32 codepoint) const {
  if (const Glyph *cached = grapheMap_.find(codepoint))
    return *cached;

  FT_Error error = FT_Load_Glyph(face_, codepoint, FT_LOAD_RENDER);
  if (error != 0) {
    L("FT_Load_GLyph(%d) = %d", codepoint, error);
    return {vec2i(0), rect2i().clear()};
  }

  Glyph ret;
  FT_GlyphSlotRec *slot = face_->glyph;
  const FT_Bitmap &b = slot->bitmap;
  ret.origin = vec2i(slot->bitmap_left, slot->bitmap_top);
  //L("G O:(%d, %d), S(%d, %d) P%d", origin.x, origin.y, size.x, size.y,
  //  slot->bitmap.pitch);

  KP_ASSERT(b.pixel_mode == FT_PIXEL_MODE_GRAY);

  Surface::Meta smeta;
  smeta.size = vec2i(b.width, b.rows);
  smeta.format = Surface::Meta::R8;
  smeta.bytesPerPixel = 1;
  smeta.bytesPerRow = -b.pitch;
  void *pixels;
  if (b.pitch > 0) pixels = b.buffer + b.pitch * (b.rows - 1);
  else pixels = b.buffer;
  Surface bitmap(smeta, pixels);

  ret.atlas = const_cast<Atlas*>(&atlas_)->insert(&bitmap);
  const_cast<MapOf<Glyph>*>(&grapheMap_)->set(codepoint, &ret);
  return ret;
}

} // namespace harftype
} // namespace fontain
} // namespace kapusha
