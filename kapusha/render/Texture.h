#pragma once
#include "../math/types.h"
#include "../core/Shared.h"

namespace kapusha {

  class Texture : public Shared {
  public:
    struct ImageDesc {
      vec2i size;
      enum PixelFormat {
        Format_Unknown,
        Format_RGBA32,
        Format_BGRA32
      } format;
      
      ImageDesc() : size(0), format(Format_Unknown) {}
      ImageDesc(unsigned w, unsigned h, PixelFormat fmt = Format_RGBA32)
      : size(w,h), format(fmt) {}
      ImageDesc(vec2i sz, PixelFormat fmt = Format_RGBA32)
      : size(sz), format(fmt) {}
      
      bool operator==(const ImageDesc& other) const
      {
        return
          size == other.size &&
          format == other.format;
      }
      
      bool operator!=(const ImageDesc& other) const
      {
        return !(*this == other);
      }
      
      unsigned getGlFormat() const;
    };

  public:
    Texture();
    Texture(const ImageDesc& desc, unsigned existing_name = 0, bool take_ownership = false);
    ~Texture();
  
    void upload(const ImageDesc& desc, void* pixels);
    const ImageDesc& desc() const { return desc_; }

    //! \todo protected:
    unsigned name() const { return name_; }
    
  private:
    void bind() const;

    bool own_;
    unsigned name_;
    ImageDesc desc_;
  };
  
  typedef shared<Texture> STexture;

} // namespace kapusha
