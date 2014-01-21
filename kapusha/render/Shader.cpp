#include "Shader.h"

namespace kapusha {
namespace render {

shader_t::shader_t(const core::string_desc_t &string, type_e type)
  : type_(type) {
  name_ = glCreateShader(static_cast<GLenum>(type_));
  const char *str = string.begin();
  const GLint len = string.length();
  glShaderSource(name_, 1, &str, &len);
  glCompileShader(name_);
  glGetShaderiv(name_, GL_COMPILE_STATUS, &compiled_);
}

shader_t::~shader_t() { glDeleteShader(name_); }

core::String *shader_t::info_log() const {
  GLint length;
  glGetShaderiv(name_, GL_INFO_LOG_LENGTH, &length);
  core::buffer_t buffer(length);
  glGetShaderInfoLog(name_, length, 0, buffer.data());
  return new core::String(std::move(buffer));
}

} // namespace render
} // namespace kapusha
