#include <limits>
#include "Atlas.h"

#define PREALLOCATED_RECTS 16

namespace kapusha {

Atlas::Atlas(const Surface::Meta& smeta) : surface_(new Surface(smeta)),
  freeRects_(PREALLOCATED_RECTS) {
  freeRects_.push_back(rect2i(smeta.size));
}

rect2i Atlas::insert(const Surface *surface) {
  const vec2i size = surface->meta().size;
  unsigned best = freeRects_.size();
  int best_delta = std::numeric_limits<int>::max();
  for (unsigned i = 0; i < freeRects_.size(); ++i) {
    rect2i &r = freeRects_[i];
    vec2i delta = r.size() - size;

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

  // blit
  rect2i ret = freeRects_[best];
  surface_->blit(ret.min, surface);

  // and split
  rect2i newrects[2];
  const vec2i delta = ret.size() - size;
  if (delta.x > delta.y) {
    newrects[0] = rect2i(ret.min.x + size.x, ret.min.y,
                         ret.max.x,          ret.max.y);
    newrects[1] = rect2i(ret.min.x,          ret.min.y + size.y,
                         ret.min.x + size.x, ret.max.y);
  } else {
    newrects[0] = rect2i(ret.min.x,          ret.min.y + size.y,
                         ret.max.x,          ret.max.y);
    newrects[1] = rect2i(ret.min.x + size.x, ret.min.y,
                         ret.max.x,          ret.min.y + size.y);
  }

  // 0 should be the biggest one, try it first
  if (!newrects[0].isEmpty()) {
    freeRects_[best] = newrects[0];
    if (!newrects[1].isEmpty()) freeRects_.push_back(newrects[1]);
  } else freeRects_.erase(best);

  return ret;
}

void Atlas::commit(Context *context) {
  if (!sampler_.get()) sampler_.reset(new Sampler(context, surface_.get()));
  else sampler_->upload(context, surface_.get());
}
} // namespace kapusha