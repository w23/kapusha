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

void transform_t::orient_at(vec4f at, vec4f up) {
  orientation_.set_direction(at - translation_, up);
  calc_matrix();
}

void transform_t::calc_matrix() {
  matrix_ = orientation_.matrix() * mat4_translation(translation_.xyz());
}

transform_t::orientation_t::orientation_t() : matrix_(1.f) {}

void transform_t::orientation_t::set_direction(vec4f forward, vec4f up) {
  forward = normalize(forward);
  const vec4f right = cross(forward, normalize(up));
  up = normalize(cross(right, forward));
  matrix_ = mat4f(right, up, -forward);
}

void transform_t::orientation_t::rotate_head(float radians) {
  set_direction(mat4_rotation(up().xyz(), radians) * forward(), up());
}

void transform_t::orientation_t::rotate_pitch(float radians) {
  set_direction(mat4_rotation(right().xyz(), radians) * forward(), up());
}

void transform_t::orientation_t::rotate_roll(float radians) {
  set_direction(forward(), mat4_rotation(forward().xyz(), radians) * up());
}

void transform_t::orientation_t::rotate_axis(float radians, vec3f axis) {
  const mat4f M(mat4_rotation(axis.xyz(), radians));
  set_direction(M * forward(), M * up());
}

} // namespace ooo
} // namespace kapusha

