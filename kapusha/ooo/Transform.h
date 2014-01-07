#pragma once
#include <kapusha/math.h>

namespace kapusha {
namespace ooo {

/// \todo Some unit tests would be nice

/// \brief A very simple transformation description
///
/// Consists of (applied in order): scale, orientation, translation
struct transform_t {
  /// \brief Initialize identity transformation
  transform_t();

  /// \brief Get transformation matrix
  inline const mat4f &matrix() const { return matrix_; }
  inline operator const mat4f &() { return matrix_; }

  /// \brief Get transformation translation
  inline vec4f translation() const { return translation_; }

  /// \brief Get transformed right (X axis) vector
  inline vec4f right() const { return orientation_.right(); }

  /// \brief Get transformed up (Y axis) vector
  inline vec4f up() const { return orientation_.up(); }

  /// \brief Get transformed forward (-Z axis) vector
  inline vec4f forward() const { return orientation_.forward(); }

  /// \brief Get scale
  inline vec4f scale() const { return scale_; }

  /// \brief Set translation
  inline void translate_to(vec4f to) { translation_ = to; }

  /// \brief Move by vector
  inline void translate_by(vec4f by) { translation_ += by; }

  /// \brief Translate to the right (along X axis) by \p units
  /// \param by Units amount to translate
  void translate_right(float by);

  /// \brief Translate up (along Y axis) by \p units
  /// \param by Units amount to translate
  void translate_up(float by);

  /// \brief Translate forward (along -Z axis) by \p units
  /// \param by Units amount to translate
  void translate_forward(float by);

  /// \brief Orient transformation so that forward points \p at
  /// \param at Point at which forward should be translated
  /// \param up General up direction, will be orthonormalized
  void orient_at(vec4f at, vec4f up);

  /// \brief Orient transformation so that forward points \p at
  ///
  /// Will preserve current up direction
  /// \param at Point at which forward should be translated
  inline void orient_at(vec4f at) { orient_at(at, up()); }

  /// \brief Orient transformatio to look \p forward
  /// \param forward New forward direction
  /// \param up General up direction, will be orthonormalized
  inline void set_direction(vec4f forward, vec4f up) {
    orientation_.set_direction(forward, up);
  }

  /// \brief Orient transformatio to look \p forward
  ///
  /// Will preserve current up direction
  /// \param forward New forward direction
  inline void set_direction(vec4f forward) { set_direction(forward, up()); }

  /// \brief Rotate around Y axis
  inline void rotate_head(float radians) { orientation_.rotate_head(radians); }

  /// \brief Rotate around X axis
  inline void rotate_pitch(float radians) { orientation_.rotate_pitch(radians); }

  /// \brief Rotate around Z axis
  inline void rotate_roll(float radians) { orientation_.rotate_roll(radians); }

  /// \brief Rotate around arbitrary axis
  inline void rotate_axis(float radians, vec3f axis) {
    orientation_.rotate_axis(radians, axis);
  }
  /// \todo set_matrix

  /// \brief Recalculate full matrix
  /// \todo Get rid of this
  void calc_matrix();

  /// \brief Orientation-only transformation
  ///
  /// Contains only rotation transform. Contains both matrix and quaternion
  /// representations that are kept in sync.
  struct orientation_t {
    /// \brief Constructs identity rotation
    orientation_t();

    /// \brief Get rotation matrix representation
    /// \returns Matrix representation, contains only rotation transform
    inline const mat4f &matrix() const { return matrix_; }
    inline operator const mat4f &() { return matrix_; }

    /// \brief Get quaternion representation
    /// \returns Quaternion representation
    inline quatf quaternion() const { return q_; }

    /// \brief Get vector to the right
    /// \return X axis direction in transformed space
    inline vec4f right() const { return matrix_.rows[0]; }

    /// \brief Get transformed up vector
    /// \return Y axis direction in transformed space
    inline vec4f up() const { return matrix_.rows[1]; }

    /// \brief Get transformed forward vector
    /// \return Z axis direction in transformed space
    inline vec4f forward() const { return -matrix_.rows[2]; }

    /// \brief Set rotation to provided quaternion
    /// \param q New rotation
    /// \warning Does not check for \p q validity
    void set(const quatf &q);

    /// \brief Set rotation by providing forward and up vectors
    ///
    /// Useful for look-at orientations.
    /// \param forward Direction forward, will be set as is
    /// \param up Direction up, will be fixed to be strictly orthogonal
    void set_direction(vec4f forward, vec4f up);

    /// \brief Set rotation by providing only direction
    ///
    /// Will preserve general up direction. Useful for look-at orientations.
    /// \warning Does not handle degenerate cases of forward=up()
    /// \param forward New direction
    inline void set_direction(vec4f forward) { set_direction(forward, up()); }

    /// \brief Rotate around transformed Y axis (left-right)
    void rotate_head(float radians);

    /// \brief Rotate around transformed X axis (up-down)
    void rotate_pitch(float radians);

    /// \brief Rotate around transformed Z axis (tilt left-right)
    void rotate_roll(float radians);

    /// \brief Rotate around provided axis
    void rotate_axis(float radians, vec3f axis);

  private:
    quatf q_;
    mat4f matrix_;
    void calc_matrix();
  }; // struct orientation_t;

private:
  vec4f translation_;
  vec4f scale_;
  orientation_t orientation_;
  mat4f matrix_;
}; // struct transform_t

} // namespace ooo
} // namespace kapusha
