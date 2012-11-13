#include "Batch.h"
#include "Material.h"
#include "Program.h"
#include "Object.h"

namespace kapusha {
  
  Object::Object(Batch *batch)
    : batch_(batch)
  {
    mview_loc_ = batch_->getMaterial()->getProgram()->getUniformLocation("um4_view");
    mproj_loc_ = batch_->getMaterial()->getProgram()->getUniformLocation("um4_proj");
  }

  void Object::draw(const math::mat4f& view, const math::mat4f& proj) const
  {
    batch_->prepare();
    batch_->getMaterial()->getProgram()->setUniformMatrix(mview_loc_,view.m);
    batch_->getMaterial()->getProgram()->setUniformMatrix(mproj_loc_,proj.m);
    batch_->draw();
  }

}