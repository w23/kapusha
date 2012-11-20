#include <iostream>

#include "../sys/System.h"
#include "../sys/Log.h"
#include "../math/types.h"
#include "../sys/IViewport.h"
#include "../sys/runGlut.h"
#include "../gl/OpenGL.h"
#include "../gl/Batch.h"
#include "../gl/Object.h"
#include "../gl/Buffer.h"
#include "../gl/Program.h"
#include "../gl/Material.h"

namespace fsquad {

  using namespace kapusha;

  class Viewport : public IViewport {
  public:
    virtual ~Viewport() {}
    virtual void init(ISystem* system);
    virtual void resize(int width, int height);
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
    
    math::vec2f rect[4] = {
      math::vec2f(-1.f, -1.f),
      math::vec2f(-1.f,  1.f),
      math::vec2f( 1.f,  1.f),
      math::vec2f( 1.f, -1.f)
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

  void Viewport::resize(int width, int height)
  {
    glViewport(0, 0, width, height);
    math::vec2f aspect;
    if (width > height)
      aspect = math::vec2f(width / height, 1.f);
    else
      aspect = math::vec2f(1.f, height / width);
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

