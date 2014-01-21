#pragma once
#include <kapusha/core.h>
#include "OpenGL.h"

namespace kapusha {
namespace render {

struct shader_t {
  enum class type_e {
    vertex = GL_VERTEX_SHADER,
    fragment = GL_FRAGMENT_SHADER
  };
  shader_t(const core::string_desc_t &string, type_e type);
  ~shader_t();

  inline GLuint name() const { return name_; }
  inline type_e type() const { return type_; }

  inline operator bool() const { return compiled_ == GL_TRUE; }
  core::String *info_log() const;

private:
  GLuint name_;
  type_e type_;
  GLint compiled_;
}; // struct shader_t

class Shader : public core::Object {
public:
  typedef core::shared<Shader> shared;

  inline Shader(const core::string_desc_t &string, shader_t::type_e type)
    : shader_(string, type) {}
  ~Shader() {}

  inline GLuint name() const { return shader_.name(); }
  inline shader_t::type_e type() const { return shader_.type(); }
  inline operator bool() const { return !!shader_; }
  inline core::String *info_log() const { return shader_.info_log(); }

private:
  shader_t shader_;
}; // class Shader

} // namespace render
} // namespace kapusha
