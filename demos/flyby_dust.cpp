#include <kapusha/viewport.h>
#include <kapusha/render.h>
#include <kapusha/ooo.h>
#include <kapusha/utils/SpectatorCameraController.h>

using namespace kapusha;
class Viewport : public IViewport {
public:
  Viewport();
  virtual ~Viewport() {}
  virtual void init(IViewportController* ctrl);
  virtual void resize(vec2i size);
  virtual void inputPointer(const PointerState& pointers);
  virtual void inputKey(const KeyState& keys);
  virtual void draw(int ms, float dt);
  virtual void close();
  
private:
  IViewportController *ctrl_;
  Context dummy_;
  Camera camera_;
  SpectatorCameraController camctl_;

  SNode root_;
};

class Ground : public Object {
public:
  Ground(Context *ctx, float size);
};

class Dust : public Object {
public:
  Dust(Context *ctx, int count, float size, float radius);
};

Ground::Ground(Context *ctx, float size) {
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
  sampler->upload(ctx, Sampler::Meta(vec2i(2, 2), Sampler::Meta::RGBA8888), tex);
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
  fsrect->load(ctx, rect, sizeof rect);
  batch->setAttribSource("av4_vtx", fsrect, 3);
  batch->setGeometry(Batch::GeometryTriangleFan, 0, 4);
  addBatch(batch);
}

Dust::Dust(Context *ctx, int count, float size, float radius) {
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
  fsrect->load(ctx, vertices, sizeof(*vertices) * count);
  delete vertices;
  batch->setAttribSource("vtx", fsrect, 3, 0, sizeof(vec3f));
  batch->setGeometry(Batch::GeometryPoints, 0, count);
  addBatch(batch);
}

///////////////////////////////////////////////////////////////////////////////
Viewport::Viewport() : camctl_(camera_) {}
void Viewport::init(IViewportController *ctrl) {
  ctrl_ = ctrl;

  Node *ndust = new Node();
  ndust->addObject(new Dust(&dummy_, 8192, .1f, 20.f));
  Node *nground = new Node();
  nground->addObject(new Ground(&dummy_, 20.f));
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
void Viewport::close() { root_.reset(); }
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
  root_->draw(&dummy_, camera_.getViewProjection());
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

IViewport *makeViewport() {
  return new Viewport;
}
