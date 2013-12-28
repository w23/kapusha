#include "Object.h"

namespace kapusha {
void Object::release() {
  if(!--retain_count_) delete this;
}
} // namespace kapusha
