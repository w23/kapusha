#include <iostream>

#include "../core/Core.h"
#include "../math/types.h"
#include "../core/IViewport.h"
#include "../render/Render.h"

namespace fsquad {

  using namespace kapusha;

  class Viewport : public IViewport {
  public:
    virtual ~Viewport() {}
    virtual void init(ISystem* system);
    virtual void resize(vec2i);
    virtual void draw(int ms, float dt);
    virtual void close();
    virtual void pointerEvent(const PointerEvent& event);
    
  private:
    ISystem *system_;
    Batch *batch_;
  };

  void Viewport::init(ISystem *system)
  {
    system_ = system;
    batch_ = new Batch();
    
    const char* svtx =
    "uniform vec2 aspect;\n"
    "attribute vec4 vtx;\n"
    "varying vec2 p;\n"
    "void main(){\n"
      "gl_Position = vtx;\n"
      "p = vtx.xy * aspect;\n"
    "}"
    ;
    const char* sfrg =
    "uniform float time;"
    "varying vec2 p;\n"
    "void main(){\n"
    "gl_FragColor = vec4(sin((p.x + time) * 8.));\n"
    "}"
    ;
    Material *mat = new Material(new Program(svtx, sfrg));
    batch_->setMaterial(mat);
    
    vec2f rect[4] = {
      vec2f(-1.f, -1.f),
      vec2f(-1.f,  1.f),
      vec2f( 1.f,  1.f),
      vec2f( 1.f, -1.f)
    };
    Buffer *fsrect = new Buffer();
    fsrect->load(rect, sizeof rect);
    batch_->setAttribSource("vtx", fsrect, 2);
    
    batch_->setGeometry(Batch::GeometryTriangleFan, 0, 4, 0);
  }

  void Viewport::close()
  {
    delete batch_;
  }

  void Viewport::resize(vec2i size)
  {
    glViewport(0, 0, size.x, size.y);
    vec2f aspect((float)size.x / size.y, 1.f);
    batch_->getMaterial()->setUniform("aspect", aspect);
  }

  void Viewport::draw(int ms, float dt)
  {
    glClear(GL_COLOR_BUFFER_BIT);
    float time = ms / 1000.f;

    batch_->getMaterial()->setUniform("time", time);
    batch_->draw();
    
    system_->redraw();
  }

  void Viewport::pointerEvent(const PointerEvent &event)
  {
    
  }
  
  IViewport *makeViewport() {
    return new Viewport;
  }
} // namespace fsquad

#if SAMPLE_STANDALONE_GLUT
namespace kapusha {
int runGlut(int argc, const char* argv[], kapusha::IViewport*);
}
int main(int argc, const char* argv[])
{
  return kapusha::runGlut(argc, argv, new fsquad::Viewport);
}
#endif

