#include <kapusha/app.h>
#include <kapusha/viewport.h>
#include <kapusha/render.h>
#include <kapusha/ooo.h>
#include <kapusha/utils/SpectatorCameraController.h>

using namespace kapusha;
class Viewport : public IViewport {
public:
  Viewport(IViewportController *controller);
  virtual ~Viewport() {}
  virtual void resize(vec2i size);
  virtual void inputPointer(const PointerState& pointers);
  virtual void inputKey(const KeyState& keys);
  virtual void draw(int ms, float dt);
  
private:
  IViewportController *ctrl_;
  Camera camera_;
  SpectatorCameraController camctl_;

  SNode root_;
};

class Ground : public Object {
public:
  Ground(float size);
};

class Dust : public Object {
public:
  Dust(int count, float size, float radius);
};

Ground::Ground(float size) {
  static const char* svtx =
  "uniform mat4 um4_mvp;\n"
  "attribute vec4 av4_vtx;\n"
#if KAPUSHA_GLES
  "varying mediump vec2 p;\n"
#else
  "varying vec2 p;\n"
#endif
  "void main(){\n"
    "gl_Position = um4_mvp * av4_vtx;\n"
    "p = av4_vtx.xz;\n"
  "}\n";  
  static const char* sfrg =
  "uniform sampler2D us2_floor;\n"
#if KAPUSHA_GLES
  "varying mediump vec2 p;\n"
#else
  "varying vec2 p;\n"
#endif
  "void main(){\n"
    "gl_FragColor = texture2D(us2_floor, p) * (20. - length(p)) / 20.;\n"
  "}\n";
  static u32 tex[4] = { 0xffffffff, 0, 0, 0xffffffff };
  Sampler *sampler = new Sampler(Sampler::Nearest, Sampler::Nearest);
  sampler->upload(Surface::Meta(vec2i(2, 2), Surface::Meta::RGBA8888), tex);
  Program *prog = new Program(svtx, sfrg);
  Material *mat = new Material(prog);
  mat->setUniform("us2_floor", sampler);
  Batch* batch = new Batch();
  batch->setMaterial(mat);
  vec3f rect[4] = {
    vec3f(-size, 0.f, -size),
    vec3f(-size, 0.f,  size),
    vec3f( size, 0.f,  size),
    vec3f( size, 0.f, -size)
  };
  Buffer *fsrect = new Buffer();
  fsrect->load(rect, sizeof rect);
  batch->setAttribSource("av4_vtx", fsrect, 3);
  batch->setGeometry(Batch::GeometryTriangleFan, 0, 4);
  addBatch(batch);
}

Dust::Dust(int count, float size, float radius) {
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
    "gl_PointSize = 20. / gl_Position.z;\n"
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
  Batch* batch = new Batch();
  batch->setMaterial(new Material(prog));
  vec3f *vertices = new vec3f[count], *p = vertices;
  for (int i = 0; i < count; ++i, ++p)
    *p = vec3f(frand(-radius, radius),
		     frand(0, radius),
		     frand(-radius, radius));
  Buffer *fsrect = new Buffer();
  fsrect->load(vertices, sizeof(*vertices) * count);
  delete vertices;
  batch->setAttribSource("vtx", fsrect, 3, 0, sizeof(vec3f));
  batch->setGeometry(Batch::GeometryPoints, 0, count);
  addBatch(batch);
}

///////////////////////////////////////////////////////////////////////////////
Viewport::Viewport(IViewportController *controller)
 : ctrl_(controller), camctl_(camera_) {
  Node *ndust = new Node();
  ndust->addObject(new Dust(8192, .1f, 20.f));
  Node *nground = new Node();
  nground->addObject(new Ground(20.f));
  root_.reset(new Node());
  root_->addChild(ndust);
  root_->addChild(nground);

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
  root_->draw(camera_.getViewProjection());
  ctrl_->requestRedraw();
}

void Viewport::inputKey(const KeyState &keys) {
  if (keys.isKeyPressed(KeyState::KeyEsc))
    ctrl_->quit(0);
}

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