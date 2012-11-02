#pragma once

#include "../math/types.h"

namespace kapusha {
  
  class Batch;
  
  class Object {
  public:
    Object(Batch *batch);
    ~Object() {}
    
    void setTransform(const math::mat4f& transform);
    const math::mat4f& getTransform() const { return transform_; }
    Batch* getBatch() const { return batch_; }
    
    void draw() const;
    
  private:
    Batch *batch_;
    math::mat4f transform_;
    int transform_location_;
  };
  
}