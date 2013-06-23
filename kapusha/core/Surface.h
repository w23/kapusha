#pragma once
#include "shared.h"
#include <kapusha/math.h>

namespace kapusha {
//! \brief Bottom-to-top (OpenGL-style) 2D surface
class Surface : public Shareable {
public:
  struct Meta {
    vec2i size;
    int bytesPerPixel, bytesPerRow;
  };

  Surface(vec2i size, int bytesPerPixel);
  Surface(Surface *parent, rect2i rect);
  ~Surface();

  inline const Meta &meta() const { return meta_; }
  inline const void *pixels() const { return pixels_; }
  inline void *pixels() { return pixels_; }
  template <typename T>
  inline T *pixels() { return reinterpret_cast<T>(pixels_); }
  template <typename T>
  inline const T *pixels() const { return reinterpret_cast<const T>(pixels_); }

  //! \todo void extend(vec2i by);

private:
  Meta meta_;
  u32 *pixels_;
  bool owner_;
};
} // namespace kapusha
