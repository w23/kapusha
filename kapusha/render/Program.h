// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "limits.h"
#include "Sampler.h"
#include "Context.h"

namespace kapusha {
  //! Simple class for handling shader programs
  class Program : public Shareable {
  public:
    struct UniformState {
      void clear();
      inline void setUniform(int location, float value) {
        setUniform(location, Uniform::Float, &value);
      }
      inline void setUniform(int location, const vec2f &value) {
        setUniform(location, Uniform::Vec2, value.tptr());
      }
      inline void setUniform(int location, const vec3f &value) {
        setUniform(location, Uniform::Vec3, value.tptr());
      }
      inline void setUniform(int location, const vec4f &value) {
        setUniform(location, Uniform::Vec4, value.tptr());
      }
      inline void setUniform(int location, const mat2f &value) {
        mat2f trans(value.transposed());
        setUniform(location, Uniform::Mat2, trans.tptr());
      }
      inline void setUniform(int location, const mat4f &value) {
        mat4f trans(value.transposed());
        setUniform(location, Uniform::Mat4, trans.tptr());
      }
      void setUniform(int location, Sampler *sampler);
      void apply(Context *ctx) const;
    private:
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
      struct UniformSampler {
        SSampler sampler;
        int location;
        inline UniformSampler() {}
        inline bool empty() const { return !sampler; }
      };
      void setUniform(int location, Uniform::Type type, const float* data);
      Uniform uniforms_[MAX_STATE_UNIFORMS];
      float storage_[MAX_STATE_UNIFORM_STORAGE];
      UniformSampler samplers_[MAX_STATE_UNIFORM_SAMPLERS];
    };
  public:
    //! Constructor compiles shaders and links program
    //! \fixme separate shader objects (for sharing and shit)
    Program(const char* vertex, const char* fragment);
    ~Program();
    void bindAttributeLocation(const char* name, int location);
    int getAttributeLocation(const char* name) const;
    int getUniformLocation(const char* name) const;
    inline void use(Context *ctx) const { ctx->useProgram(this); }
  private: // noncopyable
    Program& operator=(const Program& other) { return *this; }
    Program(const Program& other) {}
    static unsigned compileShader(unsigned name, const char* source);
    unsigned name_;
    unsigned shader_vertex_;
    unsigned shader_fragment_;
    //! \todo UniformState current_state_;
  protected:
    friend class Context;
    inline unsigned name() const { return name_; }
  }; // class Program
  typedef shared<Program> SProgram;
} // namespace kapusha
