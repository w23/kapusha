#pragma once
#include "../math/types.h"
#include "../core/Shared.h"

namespace kapusha {

  class Texture : public Shared {
  public:
    struct Meta {
      vec2i size;
      enum PixelFormat {
        None = 0,
        RGBA8888,
        BGRA8888,
        RGB565
      } format;
      
      Meta() : size(0), format(None) {}
      Meta(unsigned w, unsigned h, PixelFormat fmt = RGBA8888)
        : size(w,h), format(fmt) {}
      Meta(vec2i sz, PixelFormat fmt = RGBA8888)
        : size(sz), format(fmt) {}
      
      bool operator==(const Meta& other) const {
        return size == other.size && format == other.format;
      }
      bool operator!=(const Meta& other) const { return !(*this == other); }
    };

  public:
    Texture();
    //Texture(const Meta& Meta, unsigned existing_name = 0, bool take_ownership = false);
    ~Texture();
  
    void upload(const Meta& meta, void* pixels);
    
    const Meta& getMeta() const { return meta_; }
    unsigned getName() const { return name_; }
    
  private:
    void bind() const;

    unsigned name_;
    Meta meta_;
  };
  
  typedef shared<Texture> STexture;

} // namespace kapusha
