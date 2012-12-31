#include <string.h>
#include <string>

#include "../core/Core.h"
#include "OpenGL.h"
#include "Program.h"

namespace kapusha {

////////////////////////////////////////////////////////////////////////////////
  
  struct _ShaderInfoLogHelper {
    static int length(unsigned object)
    {
      int result;
      glGetShaderiv(object, GL_INFO_LOG_LENGTH, &result);
      return result;
    }
    static void get(unsigned object, int length, char* out)
    {
      glGetShaderInfoLog(object, length, 0, out);
    }
  };
  
  struct _ProgramInfoLogHelper {
    static int length(unsigned object)
    {
      int result;
      glGetProgramiv(object, GL_INFO_LOG_LENGTH, &result);
      return result;
    }
    static void get(unsigned object, int length, char* out)
    {
      glGetProgramInfoLog(object, length, 0, out);
    }
  };
  
  template <typename H>
  void printInfoLog(unsigned name)
  {
    int length = H::length(name);
    std::string info_log;
    info_log.resize(length);
    H::get(name, length, &*info_log.begin());
    L("there were errors:\n%s\n", info_log.c_str());
  }
  
  void printShaderInfoLog(unsigned shader)
  {
    printInfoLog<_ShaderInfoLogHelper>(shader);
  }
  
  void printProgramInfoLog(unsigned program)
  {
    printInfoLog<_ProgramInfoLogHelper>(program);
  }
  
////////////////////////////////////////////////////////////////////////////////
  
  Program::Program(const char* vertex, const char* fragment)
  : shader_vertex_(0), shader_fragment_(0), program_name_(0)
  {
    shader_vertex_ = compileShader(GL_VERTEX_SHADER, vertex);
    if (!shader_vertex_) return;
    
    shader_fragment_ = compileShader(GL_FRAGMENT_SHADER, fragment);
    if (!shader_fragment_) {
      glDeleteShader(shader_vertex_);
      shader_vertex_ = 0;
      return;
    }
    
    program_name_ = glCreateProgram();
    glAttachShader(program_name_, shader_vertex_);
    glAttachShader(program_name_, shader_fragment_);
    glLinkProgram(program_name_);
    
    // errcheck
    {
      int param;
      glGetProgramiv(program_name_, GL_LINK_STATUS, &param);
      
      if (param != GL_TRUE)
      {
        printProgramInfoLog(program_name_);
        
        glDeleteProgram(program_name_);
        glDeleteShader(shader_fragment_);
        glDeleteShader(shader_vertex_);
        shader_fragment_ = shader_vertex_ = program_name_ = 0;
      }
    }
    GL_ASSERT
  }
  
  Program::~Program()
  {
    if (program_name_)
      glDeleteProgram(program_name_);
    if (shader_fragment_)
      glDeleteShader(shader_fragment_);
    if (shader_vertex_)
      glDeleteShader(shader_vertex_);
  }
  
  unsigned Program::compileShader(unsigned type, const char* source)
  {
    unsigned shader = glCreateShader(type);
    
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    
    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE)
    {
      L("for shader [");
      char* buffer = new char[strlen(source)+1];
	  strcpy(buffer, source);
      char* s = buffer;
      for (int line = 1;; ++line)
      {
        char* next = strchr(s, '\n');
        if (next != NULL)
        {
          *next = 0;
        }
        L("%d: %s", line, s);
        if (next == NULL)
        {
          break;
        }
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
  }
  
  int Program::getAttributeLocation(const char *name) const
  {
    KP_ASSERT(program_name_);
    int loc = glGetAttribLocation(program_name_, name);
    GL_ASSERT
    KP_ASSERT(loc != -1);
    return loc;
  }
  
  int Program::getUniformLocation(const char *name) const
  {
    KP_ASSERT(program_name_);
    int loc = glGetUniformLocation(program_name_, name);
    GL_ASSERT
    KP_ASSERT(loc != -1);
    return loc;
  }
  
  void Program::setUniform(int location, const float* value,
                           int components, int count) const
  {
    KP_ASSERT(program_name_);
    
    switch (components)
    {
    case 1:
      glUniform1fv(location, count, value);
      break;
    case 2:
      glUniform2fv(location, count, value);
      break;
    case 3:
      glUniform3fv(location, count, value);
      break;
    case 4:
      glUniform4fv(location, count, value);
      break;
    default:
      KP_ASSERT(!"Invalid number of components");
    }
    GL_ASSERT
  }

  void Program::setUniform(int location, int value) const
  {
    KP_ASSERT(program_name_);
    glUniform1i(location, value);
    GL_ASSERT
  }
  
  void Program::setUniformMatrix(int location, const float* value,
                                int components, int count) const
  {
    KP_ASSERT(program_name_);
    
    switch (components)
    {
    case 2:
      glUniformMatrix2fv(location, count, GL_FALSE, value);
      break;
    case 3:
      glUniformMatrix3fv(location, count, GL_FALSE, value);
      break;
    case 4:
      glUniformMatrix4fv(location, count, GL_FALSE, value);
      break;
    default:
      KP_ASSERT(!"Invalid number of components");
    }
    GL_ASSERT
  }
} // namespace kapusha