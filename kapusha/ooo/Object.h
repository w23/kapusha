#pragma once
#include "../render/Batch.h"
#include "Node.h"

namespace kapusha {
  class Object : public Node {
  public:
    Object(const SBatch &batch, const char *mvp = "um4_mvp");
    Object(const SBatch &batch, int mvp_loc);
    virtual ~Object() {}
    Batch* getBatch() const { return batch_.get(); }
    //void draw(const mat4f& view, const mat4f& proj) const;
    void draw(const mat4f& mvp) const;
  private:
    SBatch batch_;
    int mvp_loc_;
  };
  
}