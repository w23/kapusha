#include "../core/Core.h"
#include "Node.h"

namespace kapusha {
  
  void Node::render(const Camera& cam, const mat4f &m)
  {
    //! \fixme check for this node visibility
    
    mat4f current(getMatrix() * m);
    Node *n = child_.get();
    while (n)
    {
      n->render(cam, current);
      n = n->next_.get();
    }
  }
  
  void Node::addChild(Node *node)
  {
    KP_ASSERT(!node->next_.get());
    node->next_ = child_;
    child_ = node;
  }
  
} // namespace kapusha