#include "assert.h"
#include "Surface.h"

namespace kapusha {

Surface::Surface(vec2i size, int bytesPerPixel) : owner_(true) {
  meta_.size = size;
  meta_.bytesPerPixel = bytesPerPixel;
  // enforce 4-bytes alignment
  meta_.bytesPerRow = size.x * bytesPerPixel;
  meta_.bytesPerRow += 4 - (meta_.bytesPerRow & 3);
  pixels_ = new u32[meta_.bytesPerRow * size.y];
}

Surface::Surface(Surface *parent, rect2i rect) : owner_(false) {
  KP_ASSERT(rect2i(vec2i(0), parent->meta_.size).doesContain(rect));
  meta_.size = rect.size();
  meta_.bytesPerPixel = parent->meta_.bytesPerPixel;
  u8 *u8p = reinterpret_cast<u8*>(parent->pixels_) + rect.min.y * parent->meta_.bytesPerRow;
  pixels_ = reinterpret_cast<u32*>(u8p + rect.min.x * meta_.bytesPerPixel);
  meta_.bytesPerRow = parent->meta_.bytesPerRow - meta_.bytesPerPixel
    * (parent->meta_.size.x - meta_.size.x);
}

Surface::~Surface() { if (owner_) delete[] pixels_; }

} // namespace kapusha
