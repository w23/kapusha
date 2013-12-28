#pragma once
#include "shared.h"
#include <kapusha/math.h>

namespace kapusha {
//! \brief Bottom-to-top (OpenGL-style) 2D surface
class Surface : public Object {
public:
  struct Meta {
    vec2i size;
    enum Format {
      Unknown,
      R8, RG88, RGB565, RGB888, RGBA8888, BGRA8888,
      //! \todo RF16, RF32, ...,
      RGBAF32
    } format;
    u32 bytesPerPixel, bytesPerRow;

    inline Meta() : size(0), format(Unknown) {}
    inline Meta(vec2i _size, int _bytesPerPixel)
      : size(_size), format(Unknown), bytesPerPixel(_bytesPerPixel) { init(); }
    inline Meta(vec2i _size, Format _format = RGBA8888)
      : size(_size), format(_format) { init(); }
    inline std::size_t bytes() const { return size.y * bytesPerRow; }
    bool operator==(const Meta& other) const {
      return size == other.size && format == other.format;
    }
    bool operator!=(const Meta& other) const { return !(*this == other); }
  private:
    void init();
  };

  Surface(const Meta &meta);
  Surface(const Meta &meta, void *pixels);
  ~Surface();

  inline const Meta &meta() const { return meta_; }
  inline const void *pixels() const { return pixels_; }
  inline void *pixels() { return pixels_; }
  template <typename T>
  inline T *pixels() { return reinterpret_cast<T*>(pixels_); }
  template <typename T>
  inline const T *pixels() const { return reinterpret_cast<const T*>(pixels_); }

  void clear();
  void blit(vec2i pos, const Surface *source);

  //! \todo void extend(vec2i by);

private:
  Meta meta_;
  u32 *pixels_;
  bool owner_;
};
typedef shared<Surface> SSurface;
} // namespace kapusha
