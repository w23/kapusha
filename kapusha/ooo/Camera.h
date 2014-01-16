#pragma once
#include "Transform.h"

namespace kapusha {
namespace ooo {
using namespace math;

struct camera_t {
  camera_t();

  inline vec3f position() const { return -transform_.translation().xyz(); }
  inline vec3f forward() const { return transform_.forward().xyz(); }
  inline vec3f up() const { return transform_.up().xyz(); }
  inline vec3f right() const { return transform_.right().xyz(); }

  void look_at(vec3f pos, vec3f at, vec3f up = vec3f(0.f, 1.f, 0.f));
  inline void move_to(vec3f pos) { transform_.translate_to(-vec4f(pos)); }
  inline void move_by(vec3f units) { transform_.translate_by(-vec4f(units)); }
  inline void move_forward_by(float units) { transform_.translate_forward(-units); }
  inline void move_rigth_by(float units) { transform_.translate_right(-units); }
  inline void move_up_by(float units) { transform_.translate_up(-units); }

  void look_at(vec3f at);
  inline void set_direction(vec3f forward = vec3f(0.f, 0.f, -1.f),
    vec3f up = vec3f(0.f, 1.f, 0.f)) {
    transform_.set_direction(vec4f(forward), vec4f(up));
  }
  inline void rotate_head(float radians) { transform_.rotate_head(radians); }
  inline void rotate_pitch(float radians) { transform_.rotate_pitch(radians); }
  inline void rotate_roll(float radians) { transform_.rotate_roll(radians); }
  inline void rotate_axis(vec3f axis, float radians) {
    transform_.rotate_axis(radians, axis);
  }

  void update_aspect(vec2i size);

  void calc_matrix();
  inline const mat4f &matrix_projection() const { return projection_.matrix(); }
  inline const mat4f &matrix_view() const { return transform_.matrix(); }
  inline const mat4f &matrix() const { return view_projection_; }

  struct projection_t {
    inline projection_t() {}

    void set_perspective(float fov, float aspect, float znear, float zfar);
    void update_aspect(float aspect);
    void update_aspect(vec2i size);

    inline const mat4f &matrix() const { return matrix_; }
    inline operator const mat4f &() { return matrix_; }
  private:
    float fov_, aspect_, znear_, zfar_;
    mat4f matrix_;
  };

private:
  transform_t transform_;
  projection_t projection_;
  mat4f view_projection_;
};

} // namespace ooo
} // namespace kapusha
