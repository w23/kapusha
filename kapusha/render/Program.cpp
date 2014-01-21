// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#include <string.h>
#include "../core.h"
#include "Program.h"

namespace kapusha {
namespace render {

Program::Program(const shader_t &vertex_shader, const shader_t &fragment_shader)
  : name_(glCreateProgram()) {
  KP_ASSERT(!!vertex_shader);
  KP_ASSERT(!!fragment_shader);

  glAttachShader(name_, vertex_shader.name());
  glAttachShader(name_, fragment_shader.name());
  relink();
}

Program::~Program() { glDeleteProgram(name_); }

bool Program::relink() {
  glLinkProgram(name_);
  glGetProgramiv(name_, GL_LINK_STATUS, &linked_);
  return linked_ == GL_TRUE;
}

core::String *Program::info_log() const {
  GLint length;
  glGetProgramiv(name_, GL_INFO_LOG_LENGTH, &length);
  core::buffer_t buffer(length);
  glGetProgramInfoLog(name_, length, 0, buffer.data());
  return new core::String(std::move(buffer));
}

/*void Program::bind_attrib_location(const char *name, int location) {
  KP_ASSERT(!"this is currently broken, as relinking program is required");
  KP_ASSERT(name_);
  glBindAttribLocation(name_, location, name); GL_ASSERT
}*/

int Program::attrib_location(const char *name) const {
  KP_ASSERT(!!(*this));
  int loc = glGetAttribLocation(name_, name); GL_ASSERT
  return loc;
}
int Program::uniform_location(const char *name) const {
  KP_ASSERT(!!(*this));
  int loc = glGetUniformLocation(name_, name); GL_ASSERT
  return loc;
}

// UniformState ///////////////////////////////////////////////////////////////
void Program::UniformState::clear() {
  for (int i = 0; i < MAX_STATE_UNIFORMS; ++i)
    uniform_state_[i].type = Uniform::None;
  for (int i = 0; i < MAX_STATE_UNIFORM_SAMPLERS; ++i)
    samplers_[i].sampler.reset();
}
void Program::UniformState::set_uniform(int location, Sampler *sampler) {
  for (int i = 0; i < MAX_STATE_UNIFORM_SAMPLERS; ++i)
    if (samplers_[i].empty() || samplers_[i].location == location) {
      samplers_[i].location = location;
      samplers_[i].sampler = sampler;
      return;
    }
  KP_ASSERT(!"Not enough uniform storage for texture");
}
void Program::UniformState::set_uniform(int location, Uniform::Type type,
  const f32* data) {
  if (location < 0) return;
  //KP_ASSERT(location >= 0);
  const int size = type & Uniform::_MaskComponents;
  int offset = 0;
  for (int i = 0; i < MAX_STATE_UNIFORMS; ++i) {
    Uniform& uni = uniform_state_[i];
    if (uni.type == Uniform::None) {
      KP_ASSERT((offset+size) < MAX_STATE_UNIFORM_STORAGE);
      uni.type = type;
      uni.location = location;
    } else if (uni.location == location) {
      KP_ASSERT(type == uni.type);
    } else {
      offset += uni.type & Uniform::_MaskComponents;
      continue;
    }
    memcpy(storage_ + offset, data, size * sizeof(float));
    return;
  }
  KP_ASSERT(!"Not enough uniform slots");
}
void Program::UniformState::apply() const {
  const float *data = storage_;
  for (int i = 0; i < MAX_STATE_UNIFORMS; ++i) {
    const Uniform& u = uniform_state_[i];
    const int size = u.type & UniformState::Uniform::_MaskComponents;
    if (u.type == UniformState::Uniform::None) break;
    switch(u.type) {
      case Uniform::Float: glUniform1fv(u.location, 1, data); GL_ASSERT; break;
      case Uniform::Vec2: glUniform2fv(u.location, 1, data); GL_ASSERT; break;
      case Uniform::Vec3: glUniform3fv(u.location, 1, data); GL_ASSERT; break;
      case Uniform::Vec4: glUniform4fv(u.location, 1, data); GL_ASSERT; break;
      case Uniform::Mat2: glUniformMatrix2fv(u.location, 1, GL_FALSE, data); GL_ASSERT; break;
      //case Uniform::Mat3: glUniformMatrix3fv(u.location, 1, GL_FALSE, data); break;
      case Uniform::Mat4: glUniformMatrix4fv(u.location, 1, GL_FALSE, data); GL_ASSERT; break;
      default:
        KP_ASSERT(!"Unsupported uniform type");
    } // switch
    data += size;
  } // for state uniforms
  for (int i = 0; i < MAX_STATE_UNIFORM_SAMPLERS; ++i) {
    if (samplers_[i].empty()) break;
    glUniform1i(samplers_[i].location, i); GL_ASSERT
    samplers_[i].sampler->bind(i);
  } // for uniform samplers
} // UniformState::apply()

} // namespace render
} // namespace kapusha
