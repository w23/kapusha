#include <kapusha/app.h>
#include <kapusha/viewport.h>
#include <kapusha/render.h> 

using namespace kapusha;

class Viewport : public IViewport {
public:
  Viewport(IViewportController* ctrl);
  virtual ~Viewport() {}
  virtual void resize(vec2i);
  virtual void draw(int ms, float dt);
private:
  IViewportController *ctrl_;
  SBatch batch_;
};

Viewport::Viewport(IViewportController *ctrl) : ctrl_(ctrl) {
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
    "p = (vtx.xy - ptr) * aspect;\n"
  "}";
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
  "}";
  vec2f rect[4] = {
    vec2f(-1.f, -1.f),
    vec2f(-1.f,  1.f),
    vec2f( 1.f,  1.f),
    vec2f( 1.f, -1.f)
  };
  Buffer *fsrect = new Buffer();
  fsrect->load(rect, sizeof rect);
  Program *prog = new Program(svtx, sfrg);
  batch_ = new Batch();
  batch_->setMaterial(new Material(prog));
  batch_->setAttribSource("vtx", fsrect, 2);
  batch_->setGeometry(Batch::GeometryTriangleFan, 0, 4);
}

void Viewport::resize(vec2i size) {
  glViewport(0, 0, size.x, size.y);
  vec2f aspect((float)size.x / size.y, 1.f);
  batch_->getMaterial()->setUniform("aspect", aspect);
}
void Viewport::draw(int ms, float dt) {
  GL_ASSERT
  glClear(GL_COLOR_BUFFER_BIT);
  GL_ASSERT
//    float time = ms / 1000.f;

//    batch_->getMaterial()->setUniform("time", time);
  batch_->getMaterial()->setUniform("ptr", ctrl_->pointerState().main().getPosition());
  batch_->draw();
}

////////////////////////////////////////////////////////////////////////////////
// Application config

class ViewportFactory : public IViewportFactory {
public:
  virtual ~ViewportFactory() {}
  virtual IViewport *create(IViewportController *controller) const {
    return new Viewport(controller);
  }
  virtual const Preferences &preferences() const { return prefs_; }
private:
  Preferences prefs_;
};

ViewportFactory viewport_factory;

namespace kapusha {
  Application the_application = {
    &viewport_factory
  };
} // namespace kapusha
