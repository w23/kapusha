#include <cstring>
#include "../core/core.h"
#include "Object.h"

namespace kapusha {
  int Object::addBatch(Batch *batch, const char *mvp) {
    return addBatch(batch, batch->getMaterial()->getUniformLocation(mvp));
  }
  int Object::addBatch(Batch *batch, int mvp_loc) {
    for (int i = 0; i < MAX_OBJECT_BATCHES; ++i)
      if (!batches_[i].batch.valid()) {
        BatchAttachment &b = batches_[i];
        b.batch = batch;
        b.mvp_loc = mvp_loc;
        return i;
      }
    KP_ASSERT(!"Too many batches attached to object");
    return -1;
  }
  void Object::draw(const mat4f& mvp) const {
    for (int i = 0; i < MAX_OBJECT_BATCHES; ++i) {
      const BatchAttachment &b = batches_[i];
      if (!b.batch.valid()) break;
      b.batch->uniforms().setUniform(b.mvp_loc, mvp);
      b.batch->draw();
    }
  }
} // namespace kapusha
