#include "Batch.h"
#include "Material.h"
#include "Program.h"
#include "Object.h"

namespace kapusha {
  
  Object::Object(Batch *batch)
    : batch_(batch), transform_(1.f)
  {
    transform_location_ = batch_->getMaterial()->getProgram()->
      getUniformLocation("transform");
  }
  
  void Object::setTransform(const math::mat4f& transform)
  {
    transform_ = transform;
  }

  void Object::draw() const
  {
    batch_->prepare();
    batch_->getMaterial()->getProgram()->setUniformMatrix(transform_location_,
                                                          transform_.m);
    batch_->draw();
  }

}