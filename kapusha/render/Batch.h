#pragma once
#include "OpenGL.h"
#include "Buffer.h"
#include "Material.h"

namespace kapusha {

#define MAX_ATTRIBS 8

  class Render;

  class Batch : public Shared {
  public:
    // match GL_... types exactly
    enum GeometryType {
      GeometryPoints,
      GeometryLineList,
      GeometryLineLoop,
      GeometryLineStrip,
      GeometryTriangleList,
      GeometryTriangleStrip,
      GeometryTriangleFan,
      _GeometryTypes
    };

  public:
    Batch();
    ~Batch();

    void setMaterial(Material* material) {
      material_ = material;
    }
    
    Material* getMaterial() const { return material_.get(); }

    void setAttribSource(const char* attrib_name,
                         Buffer* buffer, unsigned components = 3,
                         unsigned offset = 0, unsigned stride = 0);

    enum IndexType {
      IndexU8 = GL_UNSIGNED_BYTE,
      IndexU16 = GL_UNSIGNED_SHORT,
      IndexU32 = GL_UNSIGNED_INT
    };

    void setGeometry(GeometryType type,
                     unsigned first, unsigned count,
                     IndexType itype = IndexU16, Buffer *index = 0) {
      indices_ = index;
      first_ = first;
      count_ = count;
      gl_geometry_type_ = type;
      indexType_ = itype;
    }

    void draw(Render *r) const;

  private:
    struct Attrib {
      int index;
      SBuffer buffer;
      int components;
      int offset;
      int stride;

      Attrib() : index(-1) {}
      void bind(Render *r) const;
      void unbind() const;
    };
    Attrib attribs_[MAX_ATTRIBS];
    
    SMaterial material_;

    SBuffer indices_;
    unsigned first_, count_;
    unsigned gl_geometry_type_;
    unsigned indexType_;
  };
  
  typedef shared<Batch> SBatch;

} // namespace kapusha
