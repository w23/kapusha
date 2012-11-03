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

using namespace kapusha;

class Viewport : public IViewport {
public:
  virtual ~Viewport() {}
  virtual void init(ISystem* system);
  virtual void resize(int width, int height);
  virtual void draw(int ms);
  virtual void pointerEvent(const PointerEvent& event);
  
private:
  ISystem *system_;
  Object* object_;
};

void Viewport::init(ISystem *system)
{
  system_ = system;
  Batch* batch = new Batch();
  
  const char* svtx =
  "attribute vec4 vtx;\n"
  "uniform mat4 mview, mproj;\n"
  "varying vec2 p;\n"
  "void main(){\n"
  "gl_Position = mproj * mview * vtx;\n"
  "p = vtx.xy;\n"
  "}"
  ;
  const char* sfrg =
  "uniform vec4 color;"
  "uniform float time;"
  "varying vec2 p;\n"
  "void main(){\n"
  "gl_FragColor = color * sin((p.x + time) * 8.);\n"
  "}"
  ;
  Material *mat = new Material(new Program(svtx, sfrg));
  mat->setUniform("color", math::vec4f(1.f, 0.f, 0.f, 0.f));
  batch->setMaterial(mat);
  
  math::vec2f rect[4] = {
    math::vec2f(-1.f, -1.f),
    math::vec2f(-1.f,  1.f),
    math::vec2f( 1.f,  1.f),
    math::vec2f( 1.f, -1.f)
  };
  Buffer *fsrect = new Buffer();
  fsrect->load(rect, sizeof rect);
  batch->setAttribSource("vtx", fsrect, 2);
  
  batch->setGeometry(Batch::GeometryTriangleFan, 0, 4, 0);
  
  object_ = new Object(batch);
}

void Viewport::resize(int width, int height)
{
  glViewport(0, 0, width, height);
}

void Viewport::draw(int ms)
{
  glClear(GL_COLOR_BUFFER_BIT);
  float time = ms / 1000.f;
  //object_->setTransform(math::mat4f().rotationAroundAxis(math::vec3f(0,0,1),time));
  object_->getBatch()->getMaterial()->setUniform("time", time);
  object_->draw(math::mat4f().rotationAroundAxis(math::vec3f(0,0,1),time), math::mat4f(1.f));
  
  system_->redraw();
}

void Viewport::pointerEvent(const PointerEvent &event)
{
  
}

int main(int argc, const char * argv[])
{
  return runGlut(argc, argv, new Viewport);
}