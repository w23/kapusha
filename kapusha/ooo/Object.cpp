#include <cstring>
#include "../core/Core.h"
#include "Batch.h"
#include "Material.h"
#include "Program.h"
#include "Object.h"
#include "Render.h"

namespace kapusha {
  int Object::addBatch(Batch *batch, const char *mvp) {
    return addBatch(batch, batch->getMaterial()->getUniformLocation(mvp));
  }
  int Object::addBatch(Batch *batch, int mvp_loc) {
    for (int i = 0; i < MAX_OBJECT_BATCHES; ++i)
      if (!batches_[i].batch) {
        BatchAttachment &b = batches_[i];
        b.batch = batch;
        // this is to assign first 16*4 bytes to mvp matrix
        b.state.setUniform(mvp_loc, mat4f());
        return i;
      }
    KP_ASSERT(!"Too many batches attached to object");
    return -1;
  }
  void Object::draw(Render *r, const mat4f& mvp) {
    mat4f mtx = mvp * frame_.getMatrix();
    for (int i = 0; i < MAX_OBJECT_BATCHES; ++i) {
      if (!batches_[i].batch) break;
      // first bytes are mvp matrix
      memcpy(batches_[i].state.storage_, &mtx, sizeof mtx);
      batches_[i].batch->draw(r, &batches_[i].state);
    }
  }
} // namespace kapusha
