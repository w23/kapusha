#pragma once
#include <kapusha/math.h>

namespace kapusha {
namespace fontain {
class IFace;
class IString {
public:
  //! \brief Atomic glyph that is rendered as a whole
  struct Glyph {
    //! \brief Range of bytes in the original utf8 string that represent
    //!        this glyph.
    unsigned start, end; 

    //! \brief Glyps bounding box in pixels relative to starting pen position.
    rect2i box;
  };

  //! \brief Desription of string layout relative to starting pen position.
  struct Layout {
    //! \brief Combined bounding box of all glyphs in pixels.
    rect2i box;

    unsigned nglyphs; //!< \brief Total number of glyphs
    const Glyph *glyphs; //!< \brief Glyphs description
  };

  virtual ~IString() {}

  //! \brief Set new content for string and recalculate layout
  //!
  //! Set new text for strign and recalculate glyphs layout
  //! \param string Pointer to a first character in a utf8-encoded string
  //! \param length Number of bytes to read from string.
  //!        -1 means that string is 0-terminated
  //! \returns Pointer to Layout struct describing the new string spatial
  //!          configuration. Valid until next setStringAndCalculate or
  //!          until IString destruction.
  virtual const Layout *setStringAndCalculate(const char *string,
                                              int length = -1) = 0;

  //! \brief Get current string layout.
  //! \returns Pointer to Layout struct describing current string spatial
  //!          configuration. Valid until next setStringAndCalculate or
  //!          until IString destruction.
  virtual const Layout *getLayout() const = 0;

  //! \brief Rasterize current string to a pixelbuffer.
  //!
  //! Target pixelbuffer is 8-bit grayscale and is expected to be zeroed.
  //! \param pix Pointer to first pixel that is bottom-left of rect
  //! \param rect Target area in pixels relative to starting pen position
  //! \param pitch Distance in bytes between consecutive lines in pixelbuffer.
  virtual void render(u8 *pix, rect2i rect, int pitch) const = 0;
};
}
}
