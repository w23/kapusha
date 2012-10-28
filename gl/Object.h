#pragma once

namespace kapusha {

  class Buffer;
  class Program;

#define MAX_ATTRIBS 8

  class Object {
  public:
    enum GeometryType {
      GeometryTriangleList,
      GeometryTriangleStrip,
      GeometryTriangleFan,
      GeometryPointList,
      GeometryLineList,
      GeometryLineStrip,
      _GeometryTypes
    };

  public:
    Object();
    ~Object();

    void setProgram(Program* program) {
      shader_program_ = program;
    }

    void setAttribSource(const char* attrib_name,
      Buffer* buffer, int components = 3, int offset = 0, int stride = 0);

    void setGeometry(Buffer *index, int first, int count, GeometryType type) {
      indices_ = index;
      first_ = first;
      count_ = count;
      gl_geometry_type_ = gl_geometry_type_table[type];
    }

    void draw() const;

  private:
    struct Attrib {
      int index;
      Buffer* buffer;
      int components;
      int offset;
      int stride;

      Attrib() : index(-1) {}
      void bind() const;
    };
    Attrib attribs_[MAX_ATTRIBS];
    
    Program *shader_program_;

    Buffer *indices_;
    int first_, count_;
    unsigned gl_geometry_type_;

    static unsigned gl_geometry_type_table[_GeometryTypes];
  };

} // namespace kapusha