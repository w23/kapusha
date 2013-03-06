#pragma once
#include "../core/Shared.h"
#include "Limits.h"
#include "Texture.h"

namespace kapusha {
  class Render;
  //! Simple class for handling shader programs
  class Program : public Shareable {
  public:
    struct UniformState {
      void clear();
      inline void setUniform(int location, float value) {
        setUniform(location, Uniform::Float, &value);
      }
      inline void setUniform(int location, const vec2f &value) {
        setUniform(location, Uniform::Vec2, &value.x);
      }
      inline void setUniform(int location, const vec3f &value) {
        setUniform(location, Uniform::Vec3, &value.x);
      }
      inline void setUniform(int location, const vec4f &value) {
        setUniform(location, Uniform::Vec4, &value.x);
      }
      inline void setUniform(int location, const mat2f &value) {
        setUniform(location, Uniform::Mat2, value.m);
      }
      inline void setUniform(int location, const mat4f &value) {
        setUniform(location, Uniform::Mat4, value.m);
      }
      void setSampler(int location, Texture *sampler);
      
      struct Uniform {
        enum Type {
          None = 0,
          Float = 0x0001,
          Vec2 = 0x0002,
          Vec3 = 0x0003,
          Vec4 = 0x0004,
          Mat2 = 0x0104,
          //Mat3 = 0x0109,
          Mat4 = 0x0110,
          //! \todo Int = 0x0201,
          _MaskComponents = 0x001f,
          _MaskType = 0x0700,
          _TypeVec = 0x0000,
          _TypeMat = 0x0100
        } type;
        int location;
        int offset;
        Uniform() : type(None) {}
      };
      Uniform uniforms_[MAX_STATE_UNIFORMS];
      float storage_[MAX_STATE_UNIFORM_STORAGE];
      struct UniformSampler {
        STexture sampler;
        int location;
        inline UniformSampler() {}
        inline bool empty() const { return !sampler; }
      };
      UniformSampler samplers_[MAX_STATE_UNIFORM_SAMPLERS];
    private:
      void setUniform(int location, Uniform::Type type, const float* data);
    };
  public:
    //! Constructor compiles shaders and links program
    Program(const char* vertex, const char* fragment);
    ~Program();
    inline bool isValid() const { return program_name_ != 0; }
    int getAttributeLocation(const char* name) const;
    int getUniformLocation(const char* name) const;
    void use(Render *render, const UniformState *new_state = 0);
    //Program& operator=(const UniformState& newState);
    
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
    //! \param components Matrix size (4 = 2x2, 9 = 3x3, 16 = 4x4); only square matrices are supported
    //! \param count Size of array, if uniform is an array
    void setUniformMatrix(int location, const float* values,
                          int components = 4, int count = 1) const;

    //! \todo protected:
    unsigned name() const { return program_name_; }
    
  private: // this object is noncopyable
    Program& operator=(const Program& other) { return *this; }
    Program(const Program& other) {}
    static unsigned compileShader(unsigned name, const char* source);
    unsigned shader_vertex_;
    unsigned shader_fragment_;
    unsigned program_name_;
    //! \todo UniformState current_state_;
  }; // class Program
  typedef shared<Program> SProgram;
} // namespace kapusha
