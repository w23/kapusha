#include <kapusha/viewport.h>
#include <kapusha/render.h>
#include <kapusha/ooo.h>
#include <kapusha/utils/SpectatorCameraController.h>

using namespace kapusha;
class Viewport : public IViewport {
public:
  Viewport() : camctl_(camera_) {}
  void init(IViewportController* ctrl);
  void resize(vec2i size);
  void draw(int ms, float dt);
  void inputPointer(const PointerState& pointers);
  void close();
private:
  Object* createGround() const;
  Object* createDust() const;
private:
  IViewportController *ctrl_;
  Object *ground_;
  Object *object_;
  Camera camera_;
  SpectatorCameraController camctl_;
};
void Viewport::inputPointer(const PointerState& pointers) {
  camctl_.pointers(pointers);
  if (pointers.main().wasPressed(PointerState::Pointer::LeftButton)) {
    ctrl_->setRelativeOnlyPointer(true);
    ctrl_->hideCursor(true);
    camctl_.enableOrientation(true);
  }
  if (pointers.main().wasReleased(PointerState::Pointer::LeftButton)) {
    ctrl_->setRelativeOnlyPointer(false);
    ctrl_->hideCursor(false);
    camctl_.enableOrientation(false);
  }
}
Object* Viewport::createGround() const {
  static const char* svtx =
  "uniform mat4 um4_mvp;\n"
  "attribute vec4 vtx;\n"
#if KAPUSHA_GLES
  "varying mediump vec2 p;\n"
#else
  "varying vec2 p;\n"
#endif
  "void main(){\n"
  "gl_Position = um4_mvp * vtx;\n"
  "p = vtx.xz;\n"
  "}";
  static const char* sfrg =
#if KAPUSHA_GLES
  "varying mediump vec2 p;\n"
#else
  "varying vec2 p;\n"
#endif
  "void main(){\n"
  "gl_FragColor = vec4(mod(floor(p.x)+floor(p.y),2.)) * (20. - length(p)) / 20.;\n"
  "}";
  Program *prog = new Program(svtx, sfrg);
  prog->bindAttributeLocation("vtx", 0);
  Batch* batch = new Batch();
  batch->setMaterial(new Material(prog));
  vec3f rect[4] = {
    vec3f(-100.f, -2.f, -100.f),
    vec3f(-100.f, -2.f,  100.f),
    vec3f( 100.f, -2.f,  100.f),
    vec3f( 100.f, -2.f, -100.f)
  };
  Buffer *fsrect = new Buffer();
  fsrect->load(rect, sizeof rect);
  batch->setAttribSource(0, fsrect, 3);
  batch->setGeometry(Batch::GeometryTriangleFan, 0, 4);
  Object *ret = new Object;
  ret->addBatch(batch);
  return ret;
}

Object* Viewport::createDust() const {
  static const char* svtx =
  "uniform mat4 um4_mvp;\n"
  "attribute vec4 vtx;\n"
#if KAPUSHA_GLES
  "varying lowp float light;\n"
#else
  "varying float light;\n"
#endif
  "void main(){\n"
  "gl_Position = um4_mvp * vtx;\n"
  "gl_PointSize = 10. / gl_Position.z;\n"
  "light = (20. - length(vtx)) / 20.;\n"
  "}"
  ;
  static const char* sfrg =
#if KAPUSHA_GLES
  "varying lowp float light;\n"
#else
  "varying float light;\n"
#endif
  "void main(){\n"
  "gl_FragColor = vec4(1.) * light;\n"
  "}"
  ;
  Program *prog = new Program(svtx, sfrg);
  prog->bindAttributeLocation("vtx", 0);
  Batch* batch = new Batch();
  batch->setMaterial(new Material(prog));
  const int particles = 8192;
  const float radius = 10.f;
  vec3f *vertices = new vec3f[particles], *p = vertices;
  for (int i = 0; i < particles; ++i, ++p)
    *p = vec3f(frand(-radius, radius),
               frand(-2, radius),
               frand(-radius, radius));
  Buffer *fsrect = new Buffer();
  fsrect->load(vertices, sizeof(*vertices) * particles);
  delete vertices;
  batch->setAttribSource(0, fsrect, 3, 0, sizeof(vec3f));
  batch->setGeometry(Batch::GeometryPoints, 0, particles);
  Object *ret = new Object;
  ret->addBatch(batch);
  return ret;
}

void Viewport::init(IViewportController *ctrl)
{
  ctrl_ = ctrl;
  
  object_ = createDust();
  ground_ = createGround();
  
  glEnable(GL_DEPTH_TEST);
  GL_ASSERT
  glEnable(GL_CULL_FACE);
  GL_ASSERT
#if !KAPUSHA_GLES
  glEnable(GL_POINT_SPRITE);
  GL_ASSERT
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  GL_ASSERT
#endif
}
void Viewport::close() {
  delete object_;
  delete ground_;
}
void Viewport::resize(vec2i size) {
  glViewport(0, 0, size.x, size.y);
  camera_.setAspect((float)size.x / (float)size.y);
}
void Viewport::draw(int ms, float dt) {
  camctl_.frame(dt, ctrl_);
  camera_.update();
  
  GL_ASSERT
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL_ASSERT
  
  ground_->draw(camera_.getViewProjection());
  object_->draw(camera_.getViewProjection());
  ctrl_->requestRedraw();
}

IViewport *makeViewport() {
  return new Viewport;
}
