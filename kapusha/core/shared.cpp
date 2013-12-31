#include "shared.h"

namespace kapusha {
namespace core {

void shared_base::reset(Object* r) {
  if (r == t_) return;
  if (t_) t_->release();
  if ((t_ = r)) t_->retain();
}

} // namespace core
} // namespace kapusha
