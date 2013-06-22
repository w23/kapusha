#include "String.h"

namespace kapusha {
namespace fontain {
namespace harftype {

String::String(Face *face) : face_(face), buffer_(hb_buffer_create())
  , glyph_info_(nullptr), glyph_pos_(nullptr) {
  layout_.box = rect2i(0);
  layout_.nglyphs = 0;
  layout_.glyphs = nullptr;
}

String::~String() {
  delete layout_.glyphs;
  hb_buffer_destroy(buffer_);
}

const IString::Layout *String::setStringAndCalculate(const char *string, int length) {
  if (length == -1) length = strlen(string);

  // add string to harfbuzz buffer
  hb_buffer_reset(buffer_);
  hb_buffer_add_utf8(buffer_, string, length, 0, length);

  // process buffer to get a stream of codepoints and their locations
  hb_shape(face_->hbFont(), buffer_, NULL, 0);

  // get them
  unsigned newglyphs;
  glyph_info_ = hb_buffer_get_glyph_infos(buffer_, &newglyphs);
  glyph_pos_ = hb_buffer_get_glyph_positions(buffer_, &newglyphs);

  // allocate layout cache
  if (newglyphs != layout_.nglyphs) {
    delete[] layout_.glyphs;
    layout_.glyphs = new Glyph[newglyphs];
    layout_.nglyphs = newglyphs;
  }
  layout_.box.clear();
  unsigned prev_cluster = 0;
  vec2i pen(0);
  for (unsigned i = 0; i < newglyphs; ++i) {
    Glyph *g = const_cast<Glyph*>(layout_.glyphs + i);
    const Face::Glyph *fg = face_->getGlyphForCodepoint(glyph_info_[i].codepoint);
    KP_ASSERT(fg); //!< \todo how to deal with nonexistent glyphs?
    g->start = prev_cluster;
    g->end = prev_cluster = glyph_info_[i].cluster;

    const vec2i offset(glyph_pos_[i].x_offset/64, glyph_pos_[i].y_offset/64);
    const vec2i advance(glyph_pos_[i].x_advance/64, glyph_pos_[i].y_advance/64);
    //L("H O(%d, %d) A(%d, %d)", offset.x, offset.y, advance.x, advance.y);
    g->box = fg->box + pen + offset;
    layout_.box.extendToContain(g->box);
    pen += advance;
  }

  return &layout_;
}

const IString::Layout *String::getLayout() const { return &layout_; }

void String::render(u8 *pix, rect2i rect, int pitch) const {
  KP_ASSERT(!"Not implemented!"); //!< \todo implement lolsol
}

} // namespace harftype
} // namespace fontain
} // namespace kapusha
