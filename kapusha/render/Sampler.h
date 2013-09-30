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
    enum FilterMode {
      Linear = GL_LINEAR,
      Nearest = GL_NEAREST,
      NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
      LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
      NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
      LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
    };
    Sampler(FilterMode magnification = Linear,
            FilterMode minification = Nearest);//LinearMipmapLinear);
    Sampler(const Surface* source,
            FilterMode magnification = Linear,
            FilterMode minification = Nearest);//LinearMipmapLinear);
    ~Sampler();
    void setMagFilter(FilterMode filter);
    inline void setMinFilter(FilterMode filter) { minification_ = filter; }
    inline void allocate(const Surface::Meta &meta) {
      upload(meta, nullptr);
    }
    inline void upload(const Surface *source) {
      upload(source->meta(), source->pixels());
    }
    void upload(const Surface::Meta &meta, const void *data);
    const Surface::Meta& meta() const { return meta_; }
    inline void bind(int unit = -1) const {
      Context::bind_sampler(this, unit);
    }
  private:
    void init(FilterMode magnification, FilterMode minification);
    unsigned name_;
    Surface::Meta meta_;
    FilterMode magnification_, minification_;
  protected:
    friend class Context;
    friend class Framebuffer;
    inline unsigned name() const { return name_; }
  }; // class Sampler
  typedef shared<Sampler> SSampler;
} // namespace kapusha
