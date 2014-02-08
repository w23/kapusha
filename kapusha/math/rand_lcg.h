#pragma once
#include <kapusha/core/types.h>

namespace kapusha {
namespace math {

/// \brief 32-bit linear congruental generator
///
/// Simplest pseudo-random number generator with 32-bit output and 64-bit state
struct rand_lcg32_t {
  inline rand_lcg32_t() : state_(0) {}
  inline rand_lcg32_t(u64 seed) : state_(seed) {}

  /// \brief Generate next pseudo-random value
  u32 get();
private:
  u64 state_;
}; // namespace rand_lcg_t

} // namespace math
} // namespace kapusha
