// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include "limits.h"
#include "Sampler.h"
#include "Context.h"
#include "Shader.h"

namespace kapusha {
namespace render {

//! Simple class for handling shader programs
class Program : public core::Object {
public:
  struct UniformState {
    void clear();
    inline void set_uniform(int location, f32 value) {
      set_uniform(location, Uniform::Float, &value);
    }
    inline void set_uniform(int location, const vec2f &value) {
      set_uniform(location, Uniform::Vec2, value.tptr());
    }
    inline void set_uniform(int location, const vec3f &value) {
      set_uniform(location, Uniform::Vec3, value.tptr());
    }
    inline void set_uniform(int location, const vec4f &value) {
      set_uniform(location, Uniform::Vec4, value.tptr());
    }
    inline void set_uniform(int location, const mat2f &value) {
      mat2f trans(transpose(value));
      set_uniform(location, Uniform::Mat2, trans.tptr());
    }
    inline void set_uniform(int location, const mat4f &value) {
      mat4f trans(transpose(value));
      set_uniform(location, Uniform::Mat4, trans.tptr());
    }
    void set_uniform(int location, Sampler *sampler);
    void apply() const;
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
      Sampler::shared sampler;
      int location;
      inline UniformSampler() {}
      inline bool empty() const { return !sampler; }
    };
    void set_uniform(int location, Uniform::Type type, const f32 *data);
    Uniform uniform_state_[MAX_STATE_UNIFORMS];
    float storage_[MAX_STATE_UNIFORM_STORAGE];
    UniformSampler samplers_[MAX_STATE_UNIFORM_SAMPLERS];
  };
public:
  typedef core::shared<Program> shared;

  Program(const shader_t &vertex_shader, const shader_t &fragment_shader);
  ~Program();

  // \todo relink void bind_attrib_location(const char* name, int location);
  bool relink();
  int attrib_location(const char* name) const;
  int uniform_location(const char* name) const;

  inline operator bool() const { return linked_ == GL_TRUE; }
  core::String *info_log() const;

private:
  GLuint name_;
  GLint linked_;

protected:
  friend class Context;
  inline GLuint name() const { return name_; }
}; // class Program

} // namespace render
} // namespace kapusha
