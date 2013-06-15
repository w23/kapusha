#include "noise.h"

namespace kapusha {
  float hash(int x) {
    // stolen from teh internets!
    x = (x<<13) ^ x;
    return (1.f - ((x*(x*x*15731+789221)+1376312589)&0x7fffffff)/1073741824.f);
  }
  float hash(vec2i p) { return hash(p.x + p.y * 27644437); }
  float lerp_noise(vec2f x) {
    vec2f f(floorf(x.x), floorf(x.y)), t(x-f);
    vec2i p(f);
    return lerp(lerp(hash(p), hash(p+vec2i(1,0)), t.x),
                lerp(hash(p+vec2i(0,1)), hash(p+vec2i(1,1)), t.x), t.y);
  }
} // namespace kapusha