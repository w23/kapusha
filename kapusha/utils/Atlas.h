#pragma once
#include <kapusha/core.h>
#include <kapusha/render.h>

namespace kapusha {
  class Atlas : public core::Object {
  public:
    typedef core::shared<Atlas> shared;
    Atlas(const core::Surface::Meta &meta);

    rect2i allocate(vec2i size, vec2i guard = vec2i(0));
    rect2i insert(const core::Surface *surface, vec2i guard = vec2i(0));

    inline const core::Surface::Meta &meta() const { return surface_->meta(); }
    inline const core::Surface *getSurface() const { return surface_.get(); }
    
    Sampler *getSampler() const;
    void commit() const;
    /// \todo dropSampler();
    /// \todo optimize(); requires keeping a list of allocated rects

  private:
    core::Surface::shared surface_;
    core::ArrayOf<rect2i> freeRects_;
    mutable SSampler sampler_;
    mutable bool dirty_;
  }; // class Atlas
} // namespace kapusha
