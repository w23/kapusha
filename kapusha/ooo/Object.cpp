#include "Batch.h"
#include "Material.h"
#include "Program.h"
#include "Object.h"
#include "Render.h"

namespace kapusha {
  Object::Object(const SBatch &batch, const char *mvp) : batch_(batch) {
    mvp_loc_ = batch_->getMaterial()->getUniformLocation(mvp);
  }
  
  Object::Object(const SBatch &batch, int mvp_loc)
  : batch_(batch), mvp_loc_(mvp_loc) {}

  //void Object::draw(const mat4f& view, const mat4f& proj) const {
  void Object::draw(const mat4f& mvp) const {
    batch_->getMaterial()->setUniform(mvp_loc_, mvp);
    batch_->draw(Render::currentRender());
  }

}
