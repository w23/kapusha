#include "rand_lcg.h"

namespace kapusha {
namespace math {

enum {
  // Knuth MMIX constants
  // taken from http://en.wikipedia.org/wiki/Linear_congruential_generator
  rand_lcg32_increment = 1442695040888963407ull,
  rand_lcg32_multiplier = 6364136223846793005ull
};

u32 rand_lcg32_t::get() {
  state_ = state_ * rand_lcg32_multiplier + rand_lcg32_increment;
  return static_cast<u32>(state_ >> 16);
}

} // namespace math
} // namespace kapusha
