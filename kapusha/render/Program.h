#pragma once
#include "../core/Shared.h"

namespace kapusha {
  
  //! Simple class for handling shader programs
  class Program : public Shared {
  public:
  /*! \todo
#define KAPUSHA_MAX_UNIFORMS 16
#define KAPUSHA_MAX_UNIFORM_STATE_SIZE 64
    struct UniformState {
      struct Variable {
        enum Type {
          None = 0,
          Float = 0x0001,
          Vec2 = 0x0002,
          Vec3 = 0x0003,
          Vec4 = 0x0004,
          Mat2 = 0x0104,
          Mat3 = 0x0109,
          Mat4 = 0x0110,
          //! \todo Int = 0x0201,

          _MaskComponents = 0x1f
        };
        int type;
        int offset;

        Variable() : type(None) {}
      };
      struct Value {
        union {
          float f;
          int i
        }
      }
      Variable vars[KAPUSHA_MAX_UNIFORMS];
      Value data[KAPUSHA_MAX_UNIFORM_STATE_SIZE];
    };
    */

  public:
    //! Constructor compiles shaders and links program
    //! \param vertex Vertex shader
    //! \param fragment Fragment shader
    Program(const char* vertex, const char* fragment);
    ~Program();
    
    bool isValid() const { return program_name_ != 0; }
    
    //! Attribute location
    int getAttributeLocation(const char* name) const;

    //! Uniform location
    int getUniformLocation(const char* name) const;
    
    //! set scalar/vector float uniform value
    //! expects: program already in use
    //! \param location Uniform location obtained via getUniformLocation
    //! \param values Pointer to new uniform data
    //! \param components Vector components (1-4)
    //! \param count Size of array, if uniform is an array
    void setUniform(int location, const float* values,
                    int components = 4, int count = 1) const;
    void setUniform(int location, int value) const;
    
    //! set uniform float matrix value
    //! expects: program already in use
    //! \param location Uniform location obtained via getUniformLocation
    //! \param values Pointer to new uniform data
    //! \param components Matrix size (2-4); only square matrices are supported
    //! \param count Size of array, if uniform is an array
    void setUniformMatrix(int location, const float* values,
                          int components = 4, int count = 1) const;

    //! \todo protected:
    unsigned name() const { return program_name_; }
    
  private:
    static unsigned compileShader(unsigned name, const char* source);

  private: // this object is noncopyable
    Program& operator=(const Program& other) { return *this; }
    Program(const Program& other) {}
    
  private:
    unsigned shader_vertex_;
    unsigned shader_fragment_;
    unsigned program_name_;
  };
  
  typedef shared<Program> SProgram;

} // namespace kapusha
