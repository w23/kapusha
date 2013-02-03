#pragma once
#include "../render/Batch.h"
#include "../render/Limits.h"
#include "Reframe.h"

namespace kapusha {
  class Object : public Shareable {
  public:
    Object() : frame_(vec3f(0.)) {}
    virtual ~Object() {}
    int addBatch(Batch *batch, const char *mvp = "um4_mvp");
    int addBatch(Batch *batch, int mvp_loc);
    void draw(Render *render, const mat4f& mvp);
  private:
    Reframe frame_;
    struct BatchAttachment {
      SBatch batch;
      Program::UniformState state;
    } batches_[MAX_OBJECT_BATCHES];
  }; // class Object
  typedef shared<Object> SObject;
} // namespace kapusha