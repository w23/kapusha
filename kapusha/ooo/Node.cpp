// kapusha/ooo
// 2012-2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#include "Node.h"

namespace kapusha {
void Node::addChild(Node *node) {
  KP_ASSERT(node);
  if (first_child_) node->insertAfterSibling(first_child_.get());
  else first_child_ = node;
  node->setParent(this);
}
void Node::addObject(Object *object) {
  if (object_) object->insertAfterSibling(object_.get());
  else object_ = object;
}
void Node::draw(Context *ctx, const mat4f &mvp) const {
  mat4f mat = mvp * frame_.getMatrix();
  Object *obj = object_.get();
  while (obj) { obj->draw(ctx, mat); obj = obj->getNextSibling(); }
  Node *node = first_child_.get();
  while (node) { node->draw(ctx, mat); node = node->getNextSibling(); }
}
} // namespace kapusha
