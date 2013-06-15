#include <kapusha/viewport.h>
#include <kapusha/render.h>
#include <kapusha/ooo.h>
#include <kapusha/utils/SpectatorCameraController.h>
#include <kapusha/utils/geometry.h>
#include <kapusha/utils/noise.h>
using namespace kapusha;

class Ground : public Object {
public:
  Ground(Context *context, int detail = 128, float size = 1000.f, float height = 200.f)
  : height_(height) {
    vec2i cells(detail);
    int vertices, indices;
    calculateXYPlaneStorage(cells, vertices, indices);
    vertex *vtx = new vertex[vertices];
    u16 *idx = new u16[indices];
    makeXYPlane(cells, vtx->pos, 6, idx, 0);
    for (int i = 0; i < vertices; ++i) {
      vec2f p(vtx[i].pos[0], vtx[i].pos[1]);
      vtx[i].pos[0] = (vtx[i].pos[0] - .5f) * size;
      vtx[i].pos[2] = -(vtx[i].pos[1] - .5f) * size;
      vtx[i].pos[1] = getHeight(vec2f(vtx[i].pos[0], vtx[i].pos[2]));
    }
    calculateNormals(vtx->pos, 6, vtx->nor, 6, idx, indices);
    Buffer *vbuffer = new Buffer(Buffer::BindingArray);
    vbuffer->load(context, vtx, sizeof(vertex) * vertices);
    Buffer *ibuffer = new Buffer(Buffer::BindingIndex);
    ibuffer->load(context, idx, sizeof(u16) * indices);
    
    Program *prog = new Program(g_shaderVertex, g_shaderFragment);
    Material *mat = new Material(prog);
    mat->setUniform("uv3_light_dir", vec3f(.5f, 1.f, .5f).normalized());
    mat->setUniform("uv3_light_color", vec3f(1.f, 1.f, .9f));
    Batch* batch = new Batch();
    batch->setMaterial(mat);
    batch->setAttribSource("av4_vertex", vbuffer, 3,  0, sizeof(vertex));
    batch->setAttribSource("av3_normal", vbuffer, 3, 12, sizeof(vertex));
    batch->setGeometry(Batch::GeometryTriangleStrip, 0,
                       indices, Batch::IndexU16, ibuffer);
    addBatch(batch);
  }
  float getHeight(vec2f at) const {
    float z = 0;
    float k = .003f, a = .5f;
    for (int j = 0; j < 8; ++j, a *= .5, k *= 2.f)
      z += lerp_noise(at * k) * a;
    return z * height_;
  }
private:
  float height_;
  struct vertex {
    float pos[3];
    float nor[3];
  };
  static const char *g_shaderVertex;
  static const char *g_shaderFragment;
};
const char* Ground::g_shaderVertex =
"uniform mat4 um4_mvp, um4_model;\n"
"attribute vec4 av4_vertex;\n"
"attribute vec3 av3_normal;\n"
"varying "
#if KAPUSHA_GLES
"lowp "
#endif
"vec3 vv3_normal;\n"
"void main(){\n"
"gl_Position = um4_mvp * av4_vertex;\n"
"vv3_normal = av3_normal;\n"
"}";
const char* Ground::g_shaderFragment =
"uniform vec3 uv3_light_dir, uv3_light_color;\n"
"varying "
#if KAPUSHA_GLES
"lowp "
#endif
"vec3 vv3_normal;\n"
"void main(){\n"
"vec3 color = vec3(.4, 1., .6) * uv3_light_color * clamp(dot(uv3_light_dir, vv3_normal), 0., 1.);\n"
"gl_FragColor = vec4(color, 1.);\n"
"}";

class Viewport : public IViewport {
public:
  Viewport() : camctl_(camera_) {}
  void init(IViewportController* ctrl, Context *context);
  void resize(vec2i size);
  void draw(int ms, float dt);
  void inputPointer(const PointerState& pointers);
  void close();
private:
  Object* createGround() const;
private:
  IViewportController *ctrl_;
  Context *context_;
  Camera camera_;
  SpectatorCameraController camctl_;
  Ground *ground_;
};
void Viewport::init(IViewportController *ctrl, Context *context) {
  ctrl_ = ctrl;
  context_ = context;
  ground_ = new Ground(context);
  glEnable(GL_DEPTH_TEST);
  GL_ASSERT
  glEnable(GL_CULL_FACE);
  GL_ASSERT
  camera_.setProjection(90.f, 1.f, .1f, 1000.f);
  camera_.lookAt(vec3f(100.f), vec3f(0.f));
  glClearColor(.7f, .9f, 1.f, 1.f);
  camctl_.setSpeed(50.f);
}
void Viewport::close() {
  delete ground_;
}
void Viewport::resize(vec2i size) {
  glViewport(0, 0, size.x, size.y);
  camera_.setAspect((float)size.x / (float)size.y);
}
void Viewport::draw(int ms, float dt) {
  camctl_.frame(dt, ctrl_);
  float h = 2.f + ground_->getHeight(camera_.position().xz());
  if (h > camera_.position().y)
    camera_.move(vec3f(0., h - camera_.position().y, 0.));
  camera_.update();
  GL_ASSERT
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL_ASSERT
  ground_->draw(context_, camera_.getViewProjection());
  ctrl_->requestRedraw();
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
