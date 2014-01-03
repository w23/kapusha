#include "Transform.h"

namespace kapusha {
namespace ooo {

transform_t::transform_t() : translation_(0.f), scale_(1.f), matrix_(1.f) {}

void transform_t::translate_right(float by) {
  translate_by(orientation_.right() * by);
}

void transform_t::translate_up(float by) {
  translate_by(orientation_.up() * by);
}

void transform_t::translate_forward(float by) {
  translate_by(orientation_.forward() * by);
}

void transform_t::calc_matrix() {
  orientation_.calc_matrix();
  matrix_ = mat4f().make_translation(translation_.xyz()) * orientation_.matrix();
}

transform_t::orientation_t::orientation_t() : q_(0.f), matrix_(1.f) {}

} // namespace ooo
} // namespace kapusha

