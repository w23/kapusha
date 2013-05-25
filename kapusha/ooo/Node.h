// kapusha/ooo
// 2012-2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "../core.h"
#include "Reframe.h"
#include "Object.h"

namespace kapusha {
  class Node;
  typedef shared<Node> SNode;
  class Node : public Shareable {
  public:
    Node() {}
    ~Node() {}
    void addChild(Node *node);
    void addObject(Object *object);
    void removeFromParent() { removeFromSiblings(); }
    void draw(Context *ctx, const mat4f &mvp) const;
  protected:
    Reframe frame_;
    SNode children_;
    SObject object_;
    KP_DECLARE_DLINKEDLIST(Node, Sibling);
  }; // class Node
} // namespace kapusha
