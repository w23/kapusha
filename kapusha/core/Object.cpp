#include "Object.h"

namespace kapusha {
namespace core {

void Object::release() {
  if(!--retain_count_) delete this;
}

} // namespace core
} // namespace kapusha
