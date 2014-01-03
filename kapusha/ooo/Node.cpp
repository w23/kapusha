#include "Node.h"

namespace kapusha {
namespace ooo {

void Node::add_child(Node *node) {
  KP_ASSERT(node);
  node->sibling_item_.insert(children_);
}

//void Node::add_object(Object *object) {
//  KP_ASSERT(object);
//  object->sibling_item_.insert(objects_);
//}

void Node::draw(const mat4f &mvp) const {
  mat4f mat = mvp * frame_.getMatrix();

//  for (auto it: objects_)
//    it->draw(mat);

  //for (auto it: children_)
  for (auto it = children_.begin(); it != children_.end(); ++it)
    it.as<Node>()->draw(mat);
}

} // namespace ooo
} // namespace kapusha
