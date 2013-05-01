#pragma once
#include "OpenGL.h"
#include "../math.h"

namespace kapusha {
  class Sampler : public Shareable {
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
      Meta(vec2i sz, PixelFormat fmt = RGBA8888)
        : size(sz), format(fmt) {}
      bool operator==(const Meta& other) const {
        return size == other.size && format == other.format;
      }
      bool operator!=(const Meta& other) const { return !(*this == other); }
    };
  public:
    enum FilterMode {
      Linear = GL_LINEAR,
      Nearest = GL_NEAREST,
      NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
      LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
      NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
      LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
    };
    Sampler(FilterMode magnification = Linear,
            FilterMode minification = LinearMipmapLinear);
    ~Sampler();
    void setMagFilter(FilterMode filter);
    void setMinFilter(FilterMode filter);
    void upload(const Meta& meta, const void* pixels);
    const Meta& getMeta() const { return meta_; }
    void bind(int unit = -1) const;
  private:
    unsigned name_;
    Meta meta_;
    FilterMode magnification_, minification_;
  }; // class Sampler
  typedef shared<Sampler> SSampler;
} // namespace kapusha
