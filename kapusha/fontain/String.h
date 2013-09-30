#pragma once
#include <kapusha/core.h>
#include <kapusha/math.h>
#include <kapusha/render.h>
#include <kapusha/utils/Atlas.h>

namespace kapusha {
namespace fontain {
class String {
public:
  struct Glyph {
    vec2i offset;
    rect2i rectInAtlas;
    u32 clusterBegin, clusterEnd;
  };
  typedef ArrayOf<Glyph> Glyphs;

  String(Atlas *atlas, u32 nglyphs);
  ~String();

  inline const Atlas &atlas() const { return *atlas_.get(); }
  inline const Sampler* getAtlasSampler() const {
    return atlas_->getSampler();
  }
  inline const Glyphs &glyphs() const { return glyphs_; }
  inline Glyphs &glyphs() { return glyphs_; }
  inline u32 length() const { return glyphs_.size(); }
  inline const rect2i &box() const { return box_; }
  inline rect2i &box() { return box_; }

private:
  SAtlas atlas_;
  rect2i box_;
  Glyphs glyphs_;
}; // class String
} // namespace fontain
} // namespace kapusha
