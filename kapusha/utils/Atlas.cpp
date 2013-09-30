#include <limits>
#include "Atlas.h"

#define PREALLOCATED_RECTS 16

namespace kapusha {

Atlas::Atlas(const Surface::Meta& smeta) : surface_(new Surface(smeta)),
  freeRects_(PREALLOCATED_RECTS), dirty_(true) {
  freeRects_.push_back(rect2i(smeta.size));
  surface_->clear();
}

rect2i Atlas::insert(const Surface *surface, vec2i guard) {
  const vec2i size = surface->meta().size + guard * 2;
  rect2i rect = allocate(size, guard);
  if (rect.isValid()) surface_->blit(rect.min, surface);
  return rect;
}
rect2i Atlas::allocate(vec2i size, vec2i guard) {
  const vec2i gsize(size + guard * 2);
  unsigned best = freeRects_.size();
  int best_delta = std::numeric_limits<int>::max();
  for (unsigned i = 0; i < freeRects_.size(); ++i) {
    rect2i &r = freeRects_[i];
    vec2i delta = r.size() - gsize;

    // does not fit at all
    if (delta.x < 0 || delta.y < 0) continue;

    // a full match is a best match
    if (delta.x == 0 && delta.y == 0) {
      best = i;
      break;
    }

    // fittest is the one that uses the most
    int min_delta = min(delta.x, delta.y);
    if (min_delta < best_delta) {
      best_delta = min_delta;
      best = i;
    }
  }

  // empty rect on fail
  if (best == freeRects_.size()) return rect2i().clear();

  // split
  rect2i ret = freeRects_[best];
  rect2i newrects[2];
  const vec2i delta = ret.size() - gsize;
  if (delta.x > delta.y) {
    newrects[0] = rect2i(ret.min.x + gsize.x, ret.min.y,
                         ret.max.x,           ret.max.y);
    newrects[1] = rect2i(ret.min.x,           ret.min.y + gsize.y,
                         ret.min.x + gsize.x, ret.max.y);
  } else {
    newrects[0] = rect2i(ret.min.x,           ret.min.y + gsize.y,
                         ret.max.x,           ret.max.y);
    newrects[1] = rect2i(ret.min.x + gsize.x, ret.min.y,
                         ret.max.x,           ret.min.y + gsize.y);
  }

  // 0 should be the biggest one, try it first
  if (newrects[0].area() != 0) {
    freeRects_[best] = newrects[0];
    if (newrects[1].area() != 0) freeRects_.push_back(newrects[1]);
  } else freeRects_.erase(best);

  dirty_ = true;
  return rect2i(ret.min + guard, ret.min + guard + size);
}

  Sampler *Atlas::getSampler() const {
    commit();
    return sampler_.get();
  }

void Atlas::commit() const {
  if (!dirty_) return;
  if (!sampler_.get()) sampler_.reset(new Sampler(surface_.get()));
  else sampler_->upload(surface_.get());
  dirty_ = false;
}
} // namespace kapusha
