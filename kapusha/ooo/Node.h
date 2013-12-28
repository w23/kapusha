#pragma once
#include "../core.h"
#include "Reframe.h"
#include "Object.h"

namespace kapusha {
  class Node;
  typedef shared<Node> SNode;
  class Node : public Shareable {
  public:
    Node() : parent_(nullptr), prevSibling_(nullptr) {}
    ~Node() {}
    void addChild(Node *node);
    void addObject(Object *object);
    void removeFromParent() { removeFromSiblings(); }
    void draw(const mat4f &mvp) const;
  protected:
    inline void setParent(Node *parent) { KP_ASSERT(!parent_); parent_ = parent; }
    Reframe frame_;
    SNode first_child_;
    Node *parent_;
    KP_DECLARE_DLINKEDLIST(Node, Sibling);
    SObject object_;
  }; // class Node
} // namespace kapusha
