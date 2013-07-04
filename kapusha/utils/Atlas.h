#pragma once
#include <kapusha/core.h>
#include <kapusha/render.h>

namespace kapusha {
class Atlas {
public:
  Atlas(const Surface::Meta &meta);

  rect2i allocate(vec2i size, vec2i guard = vec2i(0));
  rect2i insert(const Surface *surface, vec2i guard = vec2i(0));
  void commit(Context *context);

  //! \todo dropSampler();

  inline const Surface::Meta &meta() const { return surface_->meta(); }
  inline const Sampler *getSampler() const { return sampler_.get(); }
  inline const Surface *getSurface() const { return surface_.get(); }

private:
  SSurface surface_;
  SSampler sampler_;
  ArrayOf<rect2i> freeRects_;
  bool dirty_;
}; // class Atlas
} // namespace kapusha
