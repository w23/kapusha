#include <kapusha/viewport.h>
#include <kapusha/render.h>
#include <kapusha/ooo.h>
#include <kapusha/utils/SpectatorCameraController.h>
#include <kapusha/utils/geometry.h>
using namespace kapusha;

class Ground : public Object {
public:
  Ground(int detail = 128, float size = 1000.f, float height = 20.f) {
    vec2i cells(detail);
    int vertices, indices;
    calculateXYPlaneStorage(cells, vertices, indices);
    vertex *vtx = new vertex[vertices];
    u16 *idx = new u16[indices];
    makeXYPlane(cells, vtx->pos, 6, idx, 0);
    for (int i = 0; i < vertices; ++i) {
      float z = height * (frand() * .1f + sin(vtx[i].pos[0]*23.f)
                                        + sin(vtx[i].pos[1]*17.f));
      vtx[i].pos[0] = (vtx[i].pos[0] - .5f) * size;
      vtx[i].pos[2] = -(vtx[i].pos[1] - .5f) * size;
      vtx[i].pos[1] = z;
    }
    calculateNormals(vtx->pos, 6, vtx->nor, 6, idx, indices);
    Buffer *vbuffer = new Buffer(Buffer::BindingArray);
    vbuffer->load(vtx, sizeof(vertex) * vertices);
    Buffer *ibuffer = new Buffer(Buffer::BindingIndex);
    ibuffer->load(idx, sizeof(u16) * indices);
    
    Program *prog = new Program(g_shaderVertex, g_shaderFragment);
    Material *mat = new Material(prog);
    mat->setUniform("uv3_light_dir", vec3f(0.f, 1.f, 0.f).normalized());
    mat->setUniform("uv3_light_color", vec3f(1.f, 1.0f, 1.0f));
    Batch* batch = new Batch();
    batch->setMaterial(mat);
    batch->setAttribSource("av4_vertex", vbuffer, 3,  0, sizeof(vertex));
    batch->setAttribSource("av4_normal", vbuffer, 3, 12, sizeof(vertex));
    batch->setGeometry(Batch::GeometryTriangleStrip, 0,
                       indices, Batch::IndexU16, ibuffer);
    addBatch(batch);
  }
private:
  struct vertex {
    float pos[3];
    float nor[3];
  };
  static const char *g_shaderVertex;
  static const char *g_shaderFragment;
};
const char* Ground::g_shaderVertex =
"uniform mat4 um4_mvp, um4_model;\n"
"uniform vec3 uv3_light_dir, uv3_light_color;\n"
"attribute vec4 av4_vertex, av4_normal;\n"
"varying "
#if KAPUSHA_GLES
"lowp "
#endif
"vec3 vv3_color;\n"
"void main(){\n"
"gl_Position = um4_mvp * av4_vertex;\n"
//"vv3_color = uv3_light_color * dot(uv3_light_dir, (um4_model * vec4(av4_normal.xyz, .0)).xyz);\n"
"vv3_color = uv3_light_color * clamp(dot(uv3_light_dir, av4_normal.xyz), 0., 1.);\n"
//"vv3_color = av4_normal.xyz;\n"
"}";
const char* Ground::g_shaderFragment =
"varying "
#if KAPUSHA_GLES
"lowp "
#endif
"vec3 vv3_color;\n"
"void main(){\n"
"gl_FragColor = vec4(vv3_color, 1.);\n"
"}";

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
private:
  IViewportController *ctrl_;
  Camera camera_;
  SpectatorCameraController camctl_;
  Ground *ground_;
};
void Viewport::init(IViewportController *ctrl) {
  ctrl_ = ctrl;
  ground_ = new Ground();
  glEnable(GL_DEPTH_TEST);
  GL_ASSERT
  glEnable(GL_CULL_FACE);
  GL_ASSERT
  camera_.setProjection(90.f, 1.f, .1f, 500.f);
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
  camera_.update();
  GL_ASSERT
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL_ASSERT
  ground_->draw(camera_.getViewProjection());
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
