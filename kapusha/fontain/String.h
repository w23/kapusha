#pragma once
#include <kapusha/math.h>
#include <kapusha/render.h>

namespace kapusha {
namespace fontain {
class String {
public:
  struct Glyph {
    vec2i offset;
    rect2i rectInAtlas;
    u32 clusterBegin, clusterEnd;
  };

  String(Sampler *atlas, u32 nglyphs);
  ~String();

  inline Sampler* atlas() const { return atlas_.get(); }
  inline u32 length() const { return nglyphs_; }
  inline const Glyph &operator[](u32 index) const {
    KP_ASSERT(index < nglyphs_);
    return glyphs_[index];
  }
  inline Glyph &operator[](u32 index) {
    KP_ASSERT(index < nglyphs_);
    return glyphs_[index];
  }

private:
  SSampler atlas_;
  u32 nglyphs_;
  Glyph *glyphs_;
}; // class String
} // namespace fontain
} // namespace kapusha
