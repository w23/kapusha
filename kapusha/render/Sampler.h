// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "OpenGL.h"
#include "../math.h"
#include "../core.h"
#include "Context.h"

namespace kapusha {
  class Framebuffer;
  class Sampler : public Shareable {
  public:
    struct Meta {
      vec2i size;
      enum SampleFormat {
        None = 0,
        RGBA8888,
        BGRA8888,
        RGB565,
        R8
      } format;
      Meta() : size(0), format(None) {}
      Meta(vec2i sz, SampleFormat fmt = RGBA8888)
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
    inline void setMinFilter(FilterMode filter) { minification_ = filter; }
    void upload(Context *ctx, const Meta& meta, const void* pixels);
    const Meta& getMeta() const { return meta_; }
    inline void bind(Context *ctx, int unit = -1) const {
      ctx->bindSampler(this, unit);
    }
  private:
    unsigned name_;
    Meta meta_;
    FilterMode magnification_, minification_;
  protected:
    friend class Context;
    friend class Framebuffer;
    inline unsigned name() const { return name_; }
  }; // class Sampler
  typedef shared<Sampler> SSampler;
} // namespace kapusha
