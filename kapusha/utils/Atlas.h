#pragma once
#include <kapusha/core.h>
#include <kapusha/render.h>

namespace kapusha {
class Atlas {
public:
  Atlas(const Surface::Meta &meta);

  rect2i insert(const Surface *surface);
  void commit(Context *context);

  //! \todo dropSampler();

  inline const Sampler *getSampler() const { return sampler_.get(); }

private:
  SSurface surface_;
  SSampler sampler_;
  ArrayOf<rect2i> freeRects_;
  bool dirty_;
}; // class Atlas
} // namespace kapusha
