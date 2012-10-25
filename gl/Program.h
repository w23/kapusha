
#pragma once

namespace kapusha {
  
  //! Simple class for handling shader programs
  class Program {
  public:
    //! Constructor compiles shaders and links program
    //! \param vertex Vertex shader
    //! \param fragment Fragment shader
    Program(const char* vertex, const char* fragment);
    ~Program();
    
    bool isValid() const { return program_name_ != 0; }
    
    //! Make current program
    void use() const;
    
    //! Set uniform value
    //! \param name Uniform name
    //! \param value Uniform value
    void setUniform(const char* name, float value) const;
    void setUniform(const char* name, float x, float y) const;
    void setUniform(const char* name, float value[4]) const;
    void setUniformMatrix(const char* name, float value[4]) const;
    
    //! Get attribute location
    unsigned getAttributeLocation(const char* name) const;
    
  private:
    static unsigned compileShader(unsigned name, const char* source);

  private: // this object is noncopyable
    Program& operator=(const Program& other) {return *this;}
    Program(const Program& other) {}
    
  private:
    unsigned shader_vertex_;
    unsigned shader_fragment_;
    unsigned program_name_;
    
  };

} // namespace kapusha