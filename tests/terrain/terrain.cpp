#include <memory>
#include <kapusha/viewport.h>
#include <kapusha/render.h>
#include <kapusha/ooo.h>
#include <kapusha/utils/SpectatorCameraController.h>
#include <kapusha/utils/geometry.h>
#include <kapusha/utils/noise.h>
using namespace kapusha;

class Ground {
public:
  Ground(int detail = 512, float size = 1000.f, float height = 200.f)
  : height_(height) {
    vec2i cells(detail);
    int vertices, indices;
    calculateXYPlaneStorage(cells, vertices, indices);
    vertex *vtx = new vertex[vertices];
    u32 *idx = new u32[indices];
    makeXYPlane(cells, vtx->pos, 6, idx, 0);
    for (int i = 0; i < vertices; ++i) {
      vec2f p(vtx[i].pos[0], vtx[i].pos[1]);
      vtx[i].pos[0] = (vtx[i].pos[0] - .5f) * size;
      vtx[i].pos[2] = -(vtx[i].pos[1] - .5f) * size;
      vtx[i].pos[1] = getHeight(vec2f(vtx[i].pos[0], vtx[i].pos[2]));
    }
    calculateNormals(vtx->pos, 6, vtx->nor, 6, idx, indices);
    Buffer *vbuffer = new Buffer(Buffer::Binding::Array);
    vbuffer->load(vtx, sizeof(vertex) * vertices);
    Buffer *ibuffer = new Buffer(Buffer::Binding::Index);
    ibuffer->load(idx, sizeof(u32) * indices);
    
    Program *prog = new Program(g_shaderVertex, g_shaderFragment);
    Material *mat = new Material(prog);
    mat->set_uniform("uv3_light_dir", normalize(vec3f(.5f, 1.f, .5f)));
    mat->set_uniform("uv3_light_color", vec3f(1.f, 1.f, .9f));
    Batch* batch = new Batch();
    batch->set_material(mat);
    batch->set_attrib_source("av4_vertex", vbuffer, 3,  0, sizeof(vertex));
    batch->set_attrib_source("av3_normal", vbuffer, 3, 12, sizeof(vertex));
    batch->set_geometry(Batch::Geometry::TriangleStrip, 0,
                       indices, Batch::Index::U32, ibuffer);
    batch_ = batch;
    loc_mvp_ = mat->get_uniform_location("um4_mvp");
  }
  float getHeight(vec2f at) const {
    float z = 0;
    float k = .003f, a = .5f;
    for (int j = 0; j < 8; ++j, a *= .5, k *= 2.f)
      z += lerp_noise(at * k) * a;
    return z * height_;
  }
  void draw(const mat4f mvp) {
    batch_->uniform_state().set_uniform(loc_mvp_, mvp);
    batch_->draw();
  }
private:
  float height_;
  struct vertex {
    float pos[3];
    float nor[3];
  };
  int loc_mvp_;
  SBatch batch_;
  static const char *g_shaderVertex;
  static const char *g_shaderFragment;
};
const char* Ground::g_shaderVertex =
"uniform mat4 um4_mvp;\n"
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
  Viewport(IViewportController* ctrl);
  void resize(vec2i size);
  void draw(int ms, float dt);
  void in_pointers(const Pointers& pointers);
private:
  Ground* createGround() const;
private:
  IViewportController *ctrl_;
  ooo::camera_t camera_;
  SpectatorCameraController camctl_;
  std::unique_ptr<Ground> ground_;
};

Viewport::Viewport(IViewportController* ctrl)
: ctrl_(ctrl), camctl_(camera_) {
  ground_.reset(new Ground());
  glEnable(GL_DEPTH_TEST);
  GL_ASSERT
  //glEnable(GL_CULL_FACE);
  GL_ASSERT
  //camera_.setProjection(90.f, 1.f, .1f, 1000.f);
  camera_.look_at(vec3f(100.f), vec3f(0.f));
  glClearColor(.7f, .9f, 1.f, 1.f);
  camctl_.set_speed(50.f);
}

void Viewport::resize(vec2i size) {
  glViewport(0, 0, size.x, size.y);
  camera_.update_aspect(size);
}

void Viewport::draw(int ms, float dt) {
  camctl_.frame(dt, ctrl_);
  float h = 2.f + ground_->getHeight(camera_.position().xz());
  if (h > camera_.position().y)
    camera_.move_by(vec3f(0., h - camera_.position().y, 0.));
  camera_.calc_matrix();
  GL_ASSERT
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL_ASSERT
  ground_->draw(camera_.matrix());

  //ooo::transform_t tr;
  //ooo::camera_t::projection_t proj;
  //proj.set_perspective(60.f, 1.f, .1f, 1000.f);
  //ground_->draw(proj.matrix());//mat4f().make_translation(vec3f(0.f, 0.f, -100.f)));
}

void Viewport::in_pointers(const Pointers &pointers) {
  camctl_.pointers(pointers);
  if (pointers.main().was_pressed(Pointers::Pointer::LeftButton)) {
    ctrl_->grab_input(true);
    camctl_.enable_orientation(true);
  }
  if (pointers.main().was_released(Pointers::Pointer::LeftButton)) {
    ctrl_->grab_input(false);
    camctl_.enable_orientation(false);
  }
}

////////////////////////////////////////////////////////////////////////////////
// Application config

class ViewportFactory : public IViewportFactory {
public:
  ViewportFactory() {
    p_.window_title = "kapusha test: terrain";
    p_.flags = Preferences::FlagOpenGL2_1;
  }
  virtual ~ViewportFactory() {}
  virtual IViewport *create(IViewportController *controller) const {
    return new Viewport(controller);
  }
  virtual const Preferences &preferences() const {
    return p_;
  }

private:
  Preferences p_;
};

const kapusha::IViewportFactory *kapusha_main(kapusha::core::StringArray *args) {
  static const ViewportFactory factory;
  return &factory;
}
