#pragma once
#include "../render/Batch.h"
#include "../render/Limits.h"
#include "Reframe.h"

namespace kapusha {
  class Object : public Shareable {
  public:
    Object() : frame_(vec3f(0.)) {}
    virtual ~Object() {}
    inline Reframe& frame() { return frame_; }
    inline const Reframe& frame() const { return frame_; }
    void clearBatches();
    int addBatch(Batch *batch, const char *mvp = "um4_mvp");
    int addBatch(Batch *batch, int mvp_loc);
    inline void setAabb(const rect4f aabb) { aabb_ = aabb; }
    inline const rect4f &getAabb() const { return aabb_; }
    void draw(const mat4f& mvp);
  private:
    Reframe frame_;
    rect4f aabb_;
    //float boundingRadius_;
    struct BatchAttachment {
      SBatch batch;
      int mvp_loc;
    } batches_[MAX_OBJECT_BATCHES];
  }; // class Object
  typedef shared<Object> SObject;
} // namespace kapusha
