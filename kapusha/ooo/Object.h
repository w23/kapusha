#pragma once
#include "../render/Batch.h"
#include "../render/limits.h"
#include "Reframe.h"

namespace kapusha {
  //! Object is a collection of batches 
  class Object : public Shareable {
  public:
    Object() {}
    virtual ~Object() {}
    int addBatch(Batch *batch, const char *mvp = "um4_mvp");
    int addBatch(Batch *batch, int mvp_loc);
    void removeFromParent() { removeFromSiblings(); }
    void draw(Context *ctx, const mat4f& mvp) const;
  private:
    struct BatchAttachment {
      SBatch batch;
      int mvp_loc;
    } batches_[MAX_OBJECT_BATCHES];
    KP_DECLARE_DLINKEDLIST(Object, Sibling);
  }; // class Object
  typedef shared<Object> SObject;
} // namespace kapusha
