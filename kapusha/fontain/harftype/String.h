#pragma once
#include "../IString.h"
#include "Face.h"

namespace kapusha {
namespace fontain {
namespace harftype {

class String : public IString {
protected:
  friend class Face;
  String(Face *face);
public:
  virtual ~String();
  virtual const Layout *setStringAndCalculate(const char *string, int length);
  virtual const Layout *getLayout() const;
  virtual void render(u8 *pix, rect2i rect, int pitch) const;
  
private:
  shared<Face> face_;
  Layout layout_;
  hb_buffer_t *buffer_;
  hb_glyph_info_t *glyph_info_;
  hb_glyph_position_t *glyph_pos_;
}; // class String
} // namespace harftype
} // namespace fontain
} // namespace kapusha
