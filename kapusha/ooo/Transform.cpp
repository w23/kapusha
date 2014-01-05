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
  matrix_ = orientation_.matrix() * mat4f().make_translation(translation_.xyz());
}

transform_t::orientation_t::orientation_t() : q_(0.f), matrix_(1.f) {}

void transform_t::orientation_t::set(const quatf &q) {
  q_ = q;
  calc_matrix();
}

void transform_t::orientation_t::set_direction(vec4f forward, vec4f up) {
  vec4f right = up.normalized().cross(forward.normalize());
  up = forward.cross(right);
  matrix_ = mat4f(right, up, forward);
  q_ = quatf(matrix_);
}

void transform_t::orientation_t::rotate_head(float radians) {
  q_ *= quatf(radians, up().xyz());
  calc_matrix();
}

void transform_t::orientation_t::rotate_pitch(float radians) {
  q_ *= quatf(radians, right().xyz());
  q_.normalize();
  calc_matrix();
}

void transform_t::orientation_t::rotate_roll(float radians) {
  q_ = quatf(radians, forward().xyz()) * q_;
  q_.normalize();
  calc_matrix();
}

void transform_t::orientation_t::rotate_axis(float radians, vec3f axis) {
  q_ *= quatf(radians, axis);
  q_.normalize();
  calc_matrix();
}

void transform_t::orientation_t::calc_matrix() {
  matrix_ = q_.calc_matrix();
}

} // namespace ooo
} // namespace kapusha

