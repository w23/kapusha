#include <kapusha/viewport.h>
#include <kapusha/render.h> 

using namespace kapusha;

class Viewport : public IViewport {
public:
  virtual ~Viewport() {}
  virtual void init(IViewportController* system);
  virtual void resize(vec2i);
  virtual void draw(int ms, float dt);
  virtual void close();
  
private:
  IViewportController *system_;
  Batch *batch_;
  Render *render_;
};

void Viewport::init(IViewportController *system)
{
  render_ = new Render;
  system_ = system;
  static const char* svtx =
  "uniform vec2 aspect;\n"
  "uniform vec2 ptr;\n"
  "attribute vec4 vtx;\n"
#if KAPUSHA_GLES
  "varying mediump vec2 p;\n"
#else
  "varying vec2 p;\n"
#endif
  "void main(){\n"
    "gl_Position = vtx;\n"
    "p = (vtx.xy + ptr) * aspect;\n"
  "}"
  ;
  static const char* sfrg =
  "uniform vec2 aspect;\n"
  "uniform float time;\n"
#if KAPUSHA_GLES
  "varying mediump vec2 p;\n"
#else
  "varying vec2 p;\n"
#endif
  "void main(){\n"
  //"gl_FragColor = vec4(1.-length(ptr*aspect-p));\n"
  "gl_FragColor = vec4(abs(p),0.,0.);\n"
  "}"
  ;
  vec2f rect[4] = {
    vec2f(-1.f, -1.f),
    vec2f(-1.f,  1.f),
    vec2f( 1.f,  1.f),
    vec2f( 1.f, -1.f)
  };
  Buffer *fsrect = new Buffer();
  fsrect->load(rect, sizeof rect);
  Program *prog = new Program(svtx, sfrg);
  prog->bindAttributeLocation("vtx", 0);
  batch_ = new Batch(new Material(prog));
  batch_->setAttribSource(0, fsrect, 2);
  batch_->setGeometry(Batch::GeometryTriangleFan, 0, 4);
}

void Viewport::close()
{
  delete batch_;
  delete render_;
}

void Viewport::resize(vec2i size)
{
  glViewport(0, 0, size.x, size.y);
  vec2f aspect((float)size.x / size.y, 1.f);
  batch_->getMaterial()->setUniform("aspect", aspect);
}

void Viewport::draw(int ms, float dt)
{
  GL_ASSERT
  glClear(GL_COLOR_BUFFER_BIT);
  GL_ASSERT
//    float time = ms / 1000.f;

//    batch_->getMaterial()->setUniform("time", time);
  batch_->getMaterial()->setUniform("ptr", system_->pointerState().main().getPosition());
  batch_->draw(render_);
  
  system_->requestRedraw();
}

IViewport *makeViewport() {
  return new Viewport;
}
