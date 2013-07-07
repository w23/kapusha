// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "OpenGL.h"
#include "Buffer.h"
#include "Material.h"

namespace kapusha {
  class Batch : public Shareable {
  public:
    enum GeometryType {
      GeometryPoints = GL_POINTS,
      GeometryLineList = GL_LINES,
      GeometryLineLoop = GL_LINE_LOOP,
      GeometryLineStrip = GL_LINE_STRIP,
      GeometryTriangleList = GL_TRIANGLES,
      GeometryTriangleStrip = GL_TRIANGLE_STRIP,
      GeometryTriangleFan = GL_TRIANGLE_FAN
    };
    enum IndexType {
      IndexU8 = GL_UNSIGNED_BYTE,
      IndexU16 = GL_UNSIGNED_SHORT,
      IndexU32 = GL_UNSIGNED_INT
    };
  public:
    Batch() {}
    ~Batch() {}
    inline void setMaterial(Material* material) { material_ = material; }
    inline void setGeometry(GeometryType gtype, unsigned first, unsigned count,
                            IndexType itype = IndexU16, Buffer *index = 0) {
      indices_ = index;
      first_ = first;
      count_ = count;
      geometryType_ = gtype;
      indexType_ = itype;
    }
    void setAttribSource(int attrib_location,
                         Buffer* buffer, u32 components = 3,
                         u32 offset = 0, u32 stride = 0);
    inline void setAttribSource(const char *attrib_name,
                         Buffer* buffer, u32 components = 3,
                         u32 offset = 0, u32 stride = 0) {
      KP_ASSERT(material_.valid());
      setAttribSource(material_->getProgram()->getAttributeLocation(attrib_name),
                      buffer, components, offset, stride);
    }
    Material* getMaterial() const { return material_.get(); }
    Program::UniformState& uniforms() { return uniforms_; }
    void draw(Context *ctx) const;
  private:
    struct Attrib {
      int index;
      SBuffer buffer;
      int components;
      void *offset;
      int stride;
      Attrib() : index(-1) {}
      inline void bind(Context *ctx) const;
      inline void unbind() const;
    };
    Attrib attribs_[MAX_BATCH_ATTRIBS];
    SMaterial material_;
    Program::UniformState uniforms_;
    SBuffer indices_;
    unsigned first_, count_;
    unsigned geometryType_;
    unsigned indexType_;
  };
  typedef shared<Batch> SBatch;
} // namespace kapusha
