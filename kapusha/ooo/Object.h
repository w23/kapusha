#pragma once

#include "Node.h"

namespace kapusha {
  
  class Batch;
  
  class Object : public Node {
  public:
    Object(Batch *batch);
    ~Object() {}
    
    Batch* getBatch() const { return batch_.get(); }
    
    void draw(const mat4f& view, const mat4f& proj) const;
    
  private:
    SBatch batch_;
    int mview_loc_;
    int mproj_loc_;
  };
  
}