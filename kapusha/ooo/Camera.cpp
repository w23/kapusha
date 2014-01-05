#include "Camera.h"

namespace kapusha {
namespace ooo {

camera_t::camera_t() {
  projection_.set_perspective(60.f, 1.f, .1f, 1000.f);
  move_to(vec3f(0.f, 0.f, -1.f));
  calc_matrix();
}

void camera_t::look_at(vec3f pos, vec3f at, vec3f up) {
  move_to(pos);
  transform_.orient_at(vec4f(at), vec4f(up));
  calc_matrix();
}

void camera_t::update_aspect(vec2i size) {
  projection_.update_aspect(size);
  calc_matrix();
}

void camera_t::calc_matrix() {
  transform_.calc_matrix();
  view_projection_ = projection_.matrix() * transform_.matrix();
}

void camera_t::projection_t::set_perspective(
  float fov, float aspect, float znear, float zfar) {
  fov_ = fov;
  aspect_ = aspect;
  znear_ = znear;
  zfar_ = zfar;

  float f = 1.f / tanf(fov * c_kdeg2rad * .5f);
  float rdz = 1.f / (znear - zfar);
  matrix_.rows[0] = vec4f(f/aspect, 0.f, 0.f, 0.f);
  matrix_.rows[1] = vec4f(0.f, f, 0.f, 0.f);
  matrix_.rows[2] = vec4f(0.f, 0.f, (zfar+znear)*rdz, 2.f*zfar*znear*rdz);
  matrix_.rows[3] = vec4f(0.f, 0.f, -1.f, 0.f);
}

void camera_t::projection_t::update_aspect(float aspect) {
  set_perspective(fov_, aspect, znear_, zfar_);
}
void camera_t::projection_t::update_aspect(vec2i size) {
  update_aspect((float)size.x / (float)size.y);
}

} // namespace ooo
} // namespace kapusha
