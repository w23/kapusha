#pragma once
#include "Reframe.h"

//! \todo
namespace kapusha {
  class Node : public Shareable {
  public:
    Node() {}
    virtual ~Node() {}
  protected:
    Reframe frame_;
    //rect3f aabb_;
  };
} // namespace kapusha