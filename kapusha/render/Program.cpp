// kapusha/render
// 2013 (c) Ivan 'w23' Avdeev, me@w23.ru
#include <string.h>
#include "../core.h"
#include "Program.h"

namespace kapusha {
  struct _ShaderInfoLogHelper {
    static int length(unsigned object) {
      int result;
      glGetShaderiv(object, GL_INFO_LOG_LENGTH, &result);
      return result;
    }
    static void get(unsigned object, int length, char* out) {
      glGetShaderInfoLog(object, length, 0, out);
    }
  };
  struct _ProgramInfoLogHelper {
    static int length(unsigned object) {
      int result;
      glGetProgramiv(object, GL_INFO_LOG_LENGTH, &result);
      return result;
    }
    static void get(unsigned object, int length, char* out) {
      glGetProgramInfoLog(object, length, 0, out);
    }
  };
  template <typename H>
  void printInfoLog(unsigned name) {
    int length = H::length(name);
    //! \todo Scrap from mapusha
    char *info_log = new char[length+1];
    H::get(name, length, info_log);
    info_log[length] = 0;
    L("there were errors:\n%s\n", info_log);
    delete info_log;
  }
  void printShaderInfoLog(unsigned shader) {
    printInfoLog<_ShaderInfoLogHelper>(shader);
  }
  void printProgramInfoLog(unsigned program) {
    printInfoLog<_ProgramInfoLogHelper>(program);
  }
  
  Program::Program(const char* vertex, const char* fragment, Validity val)
    : name_(0), shader_vertex_(0), shader_fragment_(0), validity_(val) {
    GL_ASSERT
    shader_vertex_ = compileShader(GL_VERTEX_SHADER, vertex);
    if (!shader_vertex_) return;
    shader_fragment_ = compileShader(GL_FRAGMENT_SHADER, fragment);
    if (!shader_fragment_) {
      glDeleteShader(shader_vertex_);
      shader_vertex_ = 0;
      return;
    }
    name_ = glCreateProgram();
    glAttachShader(name_, shader_vertex_);
    glAttachShader(name_, shader_fragment_);
    glLinkProgram(name_);
    {
      int param;
      glGetProgramiv(name_, GL_LINK_STATUS, &param);
      if (param != GL_TRUE) {
        printProgramInfoLog(name_);
        glDeleteProgram(name_);
        glDeleteShader(shader_fragment_);
        glDeleteShader(shader_vertex_);
        shader_fragment_ = shader_vertex_ = name_ = 0;
      }
    }
    if (val == AssertValid) {
      GL_ASSERT
    } else {
      glGetError();
    }
  } // Program::Program
  
  Program::~Program(){
    if (name_) glDeleteProgram(name_);
    if (shader_fragment_) glDeleteShader(shader_fragment_);
    if (shader_vertex_) glDeleteShader(shader_vertex_);
  }
  
  unsigned Program::compileShader(unsigned type, const char* source) {
    unsigned shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
      L("for shader [");
      char* buffer = new char[strlen(source)+1];
	  strcpy(buffer, source);
      char* s = buffer;
      for (int line = 1;; ++line) {
        char* next = strchr(s, '\n');
        if (next != NULL) *next = 0;
        L("%d: %s", line, s);
        if (next == NULL) break;
        s = next + 1;
      }
      L("]");
	  delete buffer;
      printShaderInfoLog(shader);
      glDeleteShader(shader);
      return 0;
    }
    GL_ASSERT
    return shader;
  } // Program::compileShader
  
  void Program::bindAttributeLocation(const char *name, int location) {
    KP_ASSERT(!"this is currently broken, as relinking program is required");
    KP_ASSERT(name_);
    glBindAttribLocation(name_, location, name); GL_ASSERT
  }
  int Program::getAttributeLocation(const char *name) const {
    KP_ASSERT(name_);
    int loc = glGetAttribLocation(name_, name); GL_ASSERT
    if (validity_ == AssertValid) KP_ASSERT(loc != -1);
    return loc;
  }
  int Program::getUniformLocation(const char *name) const {
    KP_ASSERT(name_);
    int loc = glGetUniformLocation(name_, name); GL_ASSERT
    if (validity_ == AssertValid) KP_ASSERT(loc != -1);
    return loc;
  }
// UniformState ///////////////////////////////////////////////////////////////  
  void Program::UniformState::clear() {
    for (int i = 0; i < MAX_STATE_UNIFORMS; ++i)
      uniforms_[i].type = Uniform::None;
    for (int i = 0; i < MAX_STATE_UNIFORM_SAMPLERS; ++i)
      samplers_[i].sampler.reset();
  }
  void Program::UniformState::setUniform(int location, Sampler *sampler) {
    for (int i = 0; i < MAX_STATE_UNIFORM_SAMPLERS; ++i)
      if (samplers_[i].empty() || samplers_[i].location == location) {
        samplers_[i].location = location;
        samplers_[i].sampler = sampler;
        return;
      }
    KP_ASSERT(!"Not enough uniform storage for texture");
  }
  void Program::UniformState::setUniform(int location, Uniform::Type type,
                                         const float* data) {
    if (location < 0) return;
    //KP_ASSERT(location >= 0);
    const int size = type & Uniform::_MaskComponents;
    int offset = 0;
    for (int i = 0; i < MAX_STATE_UNIFORMS; ++i) {
      Uniform& uni = uniforms_[i];
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
      const Uniform& u = uniforms_[i];
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
} // namespace kapusha
