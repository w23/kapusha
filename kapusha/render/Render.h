#pragma once

#include "../core/Log.h"
#include "OpenGL.h"
#include "Buffer.h"
#include "Texture.h"
#include "Program.h"
#include "Material.h"
#include "Buffer.h"
#include "Batch.h"
#include "Object.h"

namespace kapusha {

  //! Render pipeline state singleton
  class Render {
  public:
    //! getter this from weird places
    static Render *currentRender();

  public: // helper shit
    class BoolState {
      friend class Render;
      unsigned name_;
      bool current_;
      bool new_;
    protected:
      inline BoolState(unsigned name, bool def = false)
        : name_(name), current_(def), new_(def) {}
      void commit() {
        if (new_ != current_)
        {
          if (new_)
            glEnable(name_);
          else
            glDisable(name_);
          GL_ASSERT
          current_ = new_;
        }
      }
    public:
      inline void set(bool v) {
        new_ = v;
      }
      inline void on() {
        set(true);
      }
      inline void off() {
        set(false);
      }
    }; // class BoolState

    class BufferBinding {
      friend class Render;
      unsigned type_;
      unsigned current_;
      unsigned new_;
    protected:
      inline BufferBinding(unsigned type) : type_(type), current_(0), new_(0) {}
    public:
      void commit() {
        if (new_ != current_) {
          glBindBuffer(type_, new_);
          GL_ASSERT
        }
        current_ = new_;
      }
      inline void set(unsigned name) {
        new_ = name;
      }
      inline BufferBinding& bind(const Buffer *b) {
        set(b->name());
        return *this;
      }
    }; // class BufferBinding

    class ProgramBinding {
      friend class Render;
      unsigned current_;
      unsigned new_;
    protected:
      ProgramBinding() : current_(0), new_(0) {}
    public:
      inline ProgramBinding& use(const Program *p) {
        new_ = p->name();
        return *this;
      }
      inline void commit() {
        if (current_ != new_)
        {
          glUseProgram(new_);
          GL_ASSERT
          current_ = new_;
        }
      }
    }; // class ProgramBinding

    //! \fixme 2d textures only atm
    class TextureBinding {
      friend class Render;
      //! \fixme unit is shared between 1d/2d/... textures. do something
      int unitCurrent_, unitNew_;
      unsigned current_, new_;
    
    protected:
      TextureBinding() 
        : unitCurrent_(GL_TEXTURE0), unitNew_(GL_TEXTURE0)
        , current_(0), new_(0) {}
    
    public:
      inline TextureBinding& bind(const Texture* t, int unit = -1)
      {
        unitNew_ = (unit == -1) ? unitCurrent_ : (GL_TEXTURE0 + unit);
        new_ = t->name();
        return *this;
      }
      void commit()
      {
        if (unitCurrent_ != unitNew_)
        {
          GL_ASSERT
          glActiveTexture(unitNew_);
          GL_ASSERT
          unitCurrent_ = unitNew_;
          //! \fixme suboptimal
          if (current_ == new_) {
            glBindTexture(GL_TEXTURE_2D, current_);
            GL_ASSERT
          }
        }
        if (current_ != new_)
        {
            glBindTexture(GL_TEXTURE_2D, new_);
            GL_ASSERT
            current_ = new_;
        }
      }
    }; // class TextureBinding
  
  public:
    Render();
    ~Render();

  public:
    void commit();

    // boolean states
    BoolState& cullFace() { return cullFace_; }
    BoolState& depthTest() { return depthTest_; }
    BoolState& blend() { return blend_; }

    // buffer bindings
    BufferBinding& bufferArray() { return bufferArray_; }
    BufferBinding& bufferIndex() { return bufferIndex_; }
    
    // program and material
    ProgramBinding& program() { return program_; }
    void useMaterial(Material *m) { materialNew_ = m; }

    // texture bindings
    TextureBinding& texture() { return texture_; }


  private:
    static Render *current_render_;

    // booleans
    BoolState cullFace_;
    BoolState depthTest_;
    BoolState blend_;
    //! \todo add moar!1
    
    // buffers
    BufferBinding bufferArray_;
    BufferBinding bufferIndex_;

    // program and material
    ProgramBinding program_;
    Material *materialCurrent_;
    Material *materialNew_;

    // textures
    TextureBinding texture_;
  };

} // namespace lapusha
