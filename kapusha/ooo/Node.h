#pragma once

#include "Reframe.h"

namespace kapusha {
  
  class Camera;
  class Node;
  typedef shared<Node> SNode;
  
  class Node : public Reframe
  {
  public:
    virtual ~Node() {}
    
    virtual void render(const Camera& cam, const mat4f& m);
    
    void addChild(Node *node);
    
  protected:
    SNode child, next;
    
  private:
    float binding_size_;
  };
  
} // namespace kapusha