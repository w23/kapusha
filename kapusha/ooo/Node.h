#pragma once
#include <kapusha/core/shared.h>
#include <kapusha/core/ObjectIntrusiveList.h>
#include "Reframe.h"

namespace kapusha {
namespace ooo {

class Node : public core::Object {
public:
  typedef core::shared<Node> shared;

  Node() : sibling_item_(this) {}
  ~Node() {}

  void add_child(Node *node);
//  void add_object(Object *object);

  void draw(const mat4f &mvp) const;

protected:
  Reframe frame_;
  
  core::ObjectIntrusiveList::Item sibling_item_;
  core::ObjectIntrusiveList children_;
  core::ObjectIntrusiveList objects_;
}; // class Node

} // namespace ooo
} // namespace kapusha
