#pragma once

#include "Reframe.h"

namespace kapusha {
  
  class Camera;
  class Node;
  typedef shared<Node> SNode;
  
  class Node : public Reframe
  {
  public:
    Node() : diameter_(0) {}
    virtual ~Node() {}
    
    virtual void render(const Camera& cam, const mat4f& m);
    
    void addChild(Node *node);
    
    void setBoundingDiameter(float diameter) { diameter_ = diameter; }
    
    //! \todo calculateDiameter(); from children
    
  protected:
    float diameter_;
    SNode child, next;
  };
  
} // namespace kapusha