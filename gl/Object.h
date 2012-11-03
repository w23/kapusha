#pragma once

#include "../math/types.h"

namespace kapusha {
  
  class Batch;
  
  class Object {
  public:
    Object(Batch *batch);
    ~Object() {}
    
    Batch* getBatch() const { return batch_; }
    
    void draw(const math::mat4f& view, const math::mat4f& proj) const;
    
  private:
    Batch *batch_;
    int mview_loc_;
    int mproj_loc_;
  };
  
}