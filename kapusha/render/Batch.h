#pragma once
#include "OpenGL.h"
#include "Buffer.h"
#include "Material.h"

namespace kapusha {
  class Batch : public Shareable {
  public:
    enum class Geometry {
      Points = GL_POINTS,
      LineList = GL_LINES,
      LineLoop = GL_LINE_LOOP,
      LineStrip = GL_LINE_STRIP,
      TriangleList = GL_TRIANGLES,
      TriangleStrip = GL_TRIANGLE_STRIP,
      TriangleFan = GL_TRIANGLE_FAN
    };
    enum class Index {
      U8 = GL_UNSIGNED_BYTE,
      U16 = GL_UNSIGNED_SHORT,
      U32 = GL_UNSIGNED_INT
    };
	enum class AttribType {
	  S8 = GL_BYTE,
	  U8 = GL_UNSIGNED_BYTE,
	  S16 = GL_SHORT,
	  U16 = GL_UNSIGNED_SHORT,
	  Fixed = GL_FIXED,
	  Float = GL_FLOAT
	};
	enum AttributeNormalization {
	  Normalized = GL_TRUE,
	  Unnormalized = GL_FALSE
	};

  public:
    Batch();
    ~Batch() {}
    inline void set_material(Material* material) { material_ = material; }
    inline void set_geometry(Geometry gtype, unsigned first, unsigned count,
                             Index itype = Index::U16, Buffer *index = 0) {
      indices_ = index;
      first_ = first;
      count_ = count;
      geometry_type_ = static_cast<int>(gtype);
      index_type_ = static_cast<int>(itype);
    }
    void set_attrib_source(int attrib_location,
                         Buffer* buffer, u32 components = 3,
                         u32 offset = 0, u32 stride = 0,
                         AttribType type = AttribType::Float,
						 AttributeNormalization norm = Unnormalized);
    inline void set_attrib_source(const char *attrib_name,
                         Buffer* buffer, u32 components = 3,
                         u32 offset = 0, u32 stride = 0,
                         AttribType type = AttribType::Float,
						 AttributeNormalization norm = Unnormalized) {
      KP_ASSERT(material_.valid());
      set_attrib_source(material_->program()->get_attrib_location(attrib_name),
                      buffer, components, offset, stride, type, norm);
    }
    void clear_attributes();
    Material* material() const { return material_.get(); }
    Program::UniformState& uniform_state() { return uniform_state_; }
    void draw() const;
  private:
    struct Attrib {
      int index;
      SBuffer buffer;
      int components;
	  GLenum type;
	  GLboolean normalized;
      void *offset;
      int stride;
      Attrib() : index(-1) {}
      inline void bind() const;
      inline void unbind() const;
    };
    Attrib attribs_[MAX_BATCH_ATTRIBS];
    SMaterial material_;
    Program::UniformState uniform_state_;
    SBuffer indices_;
    unsigned first_, count_;
    unsigned geometry_type_;
    unsigned index_type_;
  };
  typedef shared<Batch> SBatch;
} // namespace kapusha
