#pragma once
#include "Buffer.h"
#include "Material.h"

namespace kapusha {

#define MAX_ATTRIBS 8

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

    void setGeometry(GeometryType type,
                     unsigned first, unsigned count, Buffer *index = 0) {
      indices_ = index;
      first_ = first;
      count_ = count;
      gl_geometry_type_ = type;
    }

    void draw() const;

  private:
    struct Attrib {
      int index;
      SBuffer buffer;
      int components;
      int offset;
      int stride;

      Attrib() : index(-1) {}
      void bind() const;
      void unbind() const;
    };
    Attrib attribs_[MAX_ATTRIBS];
    
    SMaterial material_;

    SBuffer indices_;
    unsigned first_, count_;
    unsigned gl_geometry_type_;
  };
  
  typedef shared<Batch> SBatch;

} // namespace kapusha