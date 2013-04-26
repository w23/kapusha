#pragma once
#include "OpenGL.h"
#include "../math/types.h"
#include "../core/shared.h"

namespace kapusha {
  class Texture : public Shareable {
  public:
    struct Meta {
      vec2i size;
      enum PixelFormat {
        None = 0,
        RGBA8888,
        BGRA8888,
        RGB565,
        R8
      } format;
      Meta() : size(0), format(None) {}
      Meta(unsigned w, unsigned h, PixelFormat fmt = RGBA8888)
        : size(w,h), format(fmt) {}
      Meta(vec2i sz, PixelFormat fmt = RGBA8888)
        : size(sz), format(fmt) {}
      bool operator==(const Meta& other) const {
        return size == other.size && format == other.format;
      }
      bool operator!=(const Meta& other) const { return !(*this == other); }
    };

  public:
    enum MagFilter {
      Linear = GL_LINEAR,
      Nearest = GL_NEAREST
    };
    Texture(MagFilter mag = Linear);
    ~Texture();
    void upload(const Meta& meta, const void* pixels);
    const Meta& getMeta() const { return meta_; }
    unsigned getName() const { return name_; }
  private:
    void bind() const;
    unsigned name_;
    Meta meta_;
    MagFilter mag_;
  }; // class Texture
  typedef shared<Texture> STexture;
} // namespace kapusha
