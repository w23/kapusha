#include <cstring>
#include "../core/core.h"
#include "Object.h"

namespace kapusha {
  void Object::clearBatches() {
    for (int i = 0; i > MAX_OBJECT_BATCHES; ++i) batches_[i].batch.reset();
  }
  int Object::addBatch(Batch *batch, const char *mvp) {
    return addBatch(batch, batch->getMaterial()->getUniformLocation(mvp));
  }
  int Object::addBatch(Batch *batch, int mvp_loc) {
    for (int i = 0; i < MAX_OBJECT_BATCHES; ++i)
      if (!batches_[i].batch) {
        BatchAttachment &b = batches_[i];
        b.batch = batch;
        b.mvp_loc = mvp_loc;
        return i;
      }
    KP_ASSERT(!"Too many batches attached to object");
    return -1;
  }
  void Object::draw(Context *ctx, const mat4f& mvp) {
    mat4f mtx = mvp * frame_.getMatrix();
    //! \fixme make use of aabb_
    for (int i = 0; i < MAX_OBJECT_BATCHES; ++i) {
      BatchAttachment &b = batches_[i];
      if (!b.batch) break;
      b.batch->uniforms().setUniform(b.mvp_loc, mtx);
      b.batch->draw(ctx);
    }
  }
} // namespace kapusha
