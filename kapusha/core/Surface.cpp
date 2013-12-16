#include <cstring>
#include "assert.h"
#include "Surface.h"

namespace kapusha {

void Surface::Meta::init() {
  switch(format) {
    case R8: bytesPerPixel = 1; break;
    case RGB565:
    case RG88: bytesPerPixel = 2; break;
    case RGB888: bytesPerPixel = 3; break;
    case BGRA8888:
    case RGBA8888: bytesPerPixel = 4; break;
    case RGBAF32: bytesPerPixel = 16; break;
    case Unknown: {}
  }
  // enforce 4-bytes alignment
  bytesPerRow = 4 * ((size.x * bytesPerPixel + 3) / 4);
}

Surface::Surface(const Meta& meta) : meta_(meta), owner_(true) {
  pixels_ = new u32[meta_.bytes() / 4];
}
Surface::Surface(const Meta &meta, void *pixels)
  : meta_(meta), pixels_(reinterpret_cast<u32*>(pixels)), owner_(false) {}

Surface::~Surface() { if (owner_) delete[] pixels_; }

void Surface::clear() {
  memset(pixels_, 0, meta_.bytes());
}

void Surface::blit(vec2i pos, const Surface *source) {
  KP_ASSERT(meta_.format == source->meta().format);
  KP_ASSERT(meta_.bytesPerPixel == source->meta().bytesPerPixel);
  KP_ASSERT(rect2i(meta_.size).does_contain(rect2i(source->meta().size)+pos));

  const u8 *psrc = reinterpret_cast<const u8*>(source->pixels_);
  u8 *pdst = reinterpret_cast<u8*>(pixels_)
    + pos.y * meta_.bytesPerRow
    + pos.x * meta_.bytesPerPixel;

  const vec2i size = source->meta().size;
  for (int y = 0; y < size.y; ++y, psrc += source->meta().bytesPerRow,
    pdst += meta_.bytesPerRow) {
    memcpy(pdst, psrc, size.x * meta_.bytesPerPixel);
  }
}

} // namespace kapusha
