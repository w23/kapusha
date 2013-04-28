#pragma once
#include "../core.h"
#include "OpenGL.h"
#include "Buffer.h"
#include "Texture.h"
#include "Program.h"
#include "Material.h"
#include "Buffer.h"
#include "Batch.h"

namespace kapusha {
  //! Render pipeline state singleton
  class Render {
  public:
    //! getter this from weird places
    static Render *currentRender();

  public: // helper shit
    class BoolState {
      unsigned name_;
      bool current_, new_;
    protected:
      friend class Render;
      inline BoolState(unsigned name, bool def = false)
        : name_(name), current_(def), new_(def) {}
      void commit() {
        if (new_ != current_) {
          if (new_) glEnable(name_); else glDisable(name_);
          GL_ASSERT
          current_ = new_;
        }
      }
    public:
      inline void set(bool v) { new_ = v; }
      inline void enable() { set(true); }
      inline void disable() { set(false); }
      inline void on() { set(true); }
      inline void off() { set(false); }
    }; // class BoolState

    class BufferBinding {
      unsigned type_, current_, new_;
    protected:
      friend class Render;
      inline BufferBinding(unsigned type) : type_(type), current_(0), new_(0) {}
    public:
      void commit() {
        if (new_ == current_) return;
        glBindBuffer(type_, new_); GL_ASSERT
        current_ = new_;
      }
      inline void set(unsigned name) { new_ = name; }
      inline BufferBinding& bind(const Buffer *b) {
        set(b->name());
        return *this;
      }
    }; // class BufferBinding

    class ProgramBinding {
      unsigned current_, new_;
    protected:
      friend class Render;
      ProgramBinding() : current_(0), new_(0) {}
    public:
      inline ProgramBinding& use(const Program *p) {
        new_ = p->name();
        return *this;
      }
      inline void commit() {
        if (current_ == new_) return;
        glUseProgram(new_); GL_ASSERT
        current_ = new_;
      }
    }; // class ProgramBinding

    //! \fixme 2d textures only atm
    class TextureBinding {
      //! \fixme unit is shared between 1d/2d/... textures. do something
      int unitCurrent_, unitNew_;
      unsigned current_, new_;
    protected:
      friend class Render;
      TextureBinding() 
        : unitCurrent_(GL_TEXTURE0), unitNew_(GL_TEXTURE0)
        , current_(0), new_(0) {}
    public:
      inline TextureBinding& bind(const Texture* t, int unit = -1) {
        unitNew_ = (unit == -1) ? unitCurrent_ : (GL_TEXTURE0 + unit);
        new_ = t->getName();
        return *this;
      }
      void commit() {
        if (unitCurrent_ != unitNew_) {
          glActiveTexture(unitNew_); GL_ASSERT
          unitCurrent_ = unitNew_;
          //! \fixme suboptimal
          if (current_ == new_) {
            glBindTexture(GL_TEXTURE_2D, current_); GL_ASSERT
          }
        }
        if (current_ != new_) {
            glBindTexture(GL_TEXTURE_2D, new_); GL_ASSERT
            current_ = new_;
        }
      }
    }; // class TextureBinding
  
  public:
    Render();
    ~Render();
    void commit();
    BoolState& cullFace() { return cullFace_; }
    BoolState& depthTest() { return depthTest_; }
    BoolState& blend() { return blend_; }
    BufferBinding& bufferArray() { return bufferArray_; }
    BufferBinding& bufferIndex() { return bufferIndex_; }
    ProgramBinding& program() { return program_; }
    void useMaterial(Material *m) { materialNew_ = m; }
    TextureBinding& texture() { return texture_; }
    
  private:
    static Render *currentRender_;
    BoolState cullFace_;
    BoolState depthTest_;
    BoolState blend_;
    //! \todo add moar!1
    BufferBinding bufferArray_;
    BufferBinding bufferIndex_;
    ProgramBinding program_;
    //! \fixme materials don't belong here, programs are fine by themselves
    Material *materialCurrent_;
    Material *materialNew_;
    TextureBinding texture_;
  };
} // namespace lapusha
