#include "shared.h"

namespace kapusha {
namespace core {

void shared_base::reset(Object* r) {
  if (r == t_) return;
  Object *prev = t_;
  // must be nullptr when release() is done, see IntrusiveList
  t_ = nullptr;
  if (prev) prev->release();
  if ((t_ = r)) t_->retain();
}

} // namespace core
} // namespace kapusha
