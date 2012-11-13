
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

} // namespace kapusha