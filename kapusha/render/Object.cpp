#include "Batch.h"
#include "Material.h"
#include "Program.h"
#include "Object.h"

namespace kapusha {
  
  Object::Object(Batch *batch)
    : batch_(batch)
  {
    mview_loc_ = batch_->getMaterial()->getUniformLocation("um4_view");
    mproj_loc_ = batch_->getMaterial()->getUniformLocation("um4_proj");
  }

  void Object::draw(const mat4f& view, const mat4f& proj) const
  {
    batch_->getMaterial()->setUniform(mview_loc_, view);
    batch_->getMaterial()->setUniform(mproj_loc_, proj);
    batch_->draw();
  }

}