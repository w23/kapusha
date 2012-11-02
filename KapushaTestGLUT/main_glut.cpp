#include <iostream>

#ifdef __APPLE__
#include <GLUT/GLUT.h>
#else
#include <GL/glut.h>
#endif

#include "../sys/System.h"
#include "../sys/Log.h"
#include "../math/types.h"
#include "../sys/IViewport.h"
#include "../sys/runGlut.h"
#include "../gl/Batch.h"
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
  
private:
  Batch* object_;
};

void Viewport::init(ISystem *system)
{
  object_ = new Batch();
  
  const char* svtx =
  "attribute vec4 vtx;\n"
  "void main(){\n"
  "gl_Position = vtx;\n"
  "}"
  ;
  const char* sfrg =
  "uniform vec4 color;"
  "void main(){\n"
  "gl_FragColor = color;\n"
  "}"
  ;
  Material *mat = new Material(new Program(svtx, sfrg));
  mat->setUniform("color", math::vec4f(0.f, 1.f, 0.f, 0.f));
  object_->setMaterial(mat);
  
  math::vec2f rect[4] = {
    math::vec2f(-1.f, -1.f),
    math::vec2f(-1.f,  1.f),
    math::vec2f( 1.f,  1.f),
    math::vec2f( 1.f, -1.f)
  };
  Buffer *fsrect = new Buffer();
  fsrect->load(rect, sizeof rect);
  object_->setAttribSource("vtx", fsrect, 2);
  
  object_->setGeometry(Batch::GeometryTriangleFan, 0, 4, 0);

}

void Viewport::resize(int width, int height)
{
  glViewport(0, 0, width, height);
}

void Viewport::draw(int ms)
{
  object_->draw();
}

int main(int argc, const char * argv[])
{
  return runGlut(argc, argv, new Viewport);
}