#pragma once
#include <kapusha/math.h>

namespace kapusha {
namespace ooo {

/// \brief A very simple transformation description
///
/// Consists of (applied in order): scale, orientation, translation
struct transform_t {
  transform_t();

  inline const mat4f &matrix() const { return matrix_; }
  inline vec4f translation() const { return translation_; }
  inline vec4f right() const { return orientation_.right(); }
  inline vec4f up() const { return orientation_.up(); }
  inline vec4f forward() const { return orientation_.forward(); }
  inline vec4f scale() const { return scale_; }

  inline void translate_to(vec4f to) { translation_ = to; }
  inline void translate_by(vec4f by) { translation_ += by; }
  void translate_right(float by);
  void translate_up(float by);
  void translate_forward(float by);
  void orient_at(vec4f at, vec4f up);
  inline void orient_at(vec4f at) { orient_at(at, up()); }
  /// \todo rotate_head/pitch/roll
  /// \todo rotate_around
  void calc_matrix();

  struct orientation_t {
    orientation_t();

    inline const mat4f &matrix() const { return matrix_; }
    inline quatf quaternion() const { return q_; }
    inline vec4f right() const { return matrix_.rows[0]; }
    inline vec4f up() const { return matrix_.rows[1]; }
    inline vec4f forward() const { return -matrix_.rows[2]; }

    inline void set(const quatf &q) { q_ = q; }
    void set_direction(vec4f forward, vec4f up);
    inline void set_direction(vec4f at) { set_direction(at, up()); }
    /// \todo set_matrix, rotate_head/pitch/roll, ...
    inline void calc_matrix() { matrix_ = q_.calc_matrix(); }

  private:
    quatf q_;
    mat4f matrix_;
  }; // struct orientation_t;

private:
  vec4f translation_;
  vec4f scale_;
  orientation_t orientation_;
  mat4f matrix_;
}; // struct transform_t

} // namespace ooo
} // namespace kapusha
