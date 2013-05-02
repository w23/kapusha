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
                         Buffer* buffer, unsigned components = 3,
                         unsigned offset = 0, unsigned stride = 0);
    void setAttribSource(const char *attrib_name,
                         Buffer* buffer, unsigned components = 3,
                         unsigned offset = 0, unsigned stride = 0);
    Material* getMaterial() const { return material_.get(); }
    Program::UniformState& uniforms() { return uniforms_; }
    void draw() const;
  private:
    struct Attrib {
      int index;
      SBuffer buffer;
      int components;
      int offset;
      int stride;
      Attrib() : index(-1) {}
      inline void bind() const;
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
