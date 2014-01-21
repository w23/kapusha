#include <memory>
#include <kapusha/viewport.h>
#include <kapusha/render.h>
#include <kapusha/ooo.h>
#include <kapusha/utils/SpectatorCameraController.h>

using namespace kapusha;

class Viewport : public IViewport {
public:
  Viewport(IViewportController* ctrl);
  void resize(vec2i size);
  void draw(int ms, float dt);
  void in_pointers(const Pointers& pointers);

private:
  IViewportController *ctrl_;
  ooo::camera_t camera_;
  SpectatorCameraController camctl_;

  ooo::Visual::shared visual;
};

Viewport::Viewport(IViewportController* ctrl)
: ctrl_(ctrl), camctl_(camera_) {
  camera_.look_at(vec3f(10.f), vec3f(0.f, 5.f, 0.f));
  glClearColor(.7f, .9f, 1.f, 1.f);
  camctl_.set_speed(5.f);

  static const u16 indices[] = {
    0, 3, 1, 0, 2, 3,
    4, 7, 5, 4, 6, 7,
    8, 11, 9, 8, 10, 11,
    12, 15, 13, 12, 14, 15,
    16, 19, 17, 16, 18, 19,
    20, 23, 21, 20, 22, 23
  };

  static const struct vtx_t {
    vec3f vtx, nor;
  } vertices[] = {
    {vec3f( 1.f, -1.f, -1.f), vec3f(0.f, 0.f, -1.f)},
    {vec3f( 1.f,  1.f, -1.f), vec3f(0.f, 0.f, -1.f)},
    {vec3f(-1.f, -1.f, -1.f), vec3f(0.f, 0.f, -1.f)},
    {vec3f(-1.f,  1.f, -1.f), vec3f(0.f, 0.f, -1.f)},

    {vec3f(-1.f, -1.f, 1.f), vec3f(0.f, 0.f, 1.f)},
    {vec3f(-1.f,  1.f, 1.f), vec3f(0.f, 0.f, 1.f)},
    {vec3f( 1.f, -1.f, 1.f), vec3f(0.f, 0.f, 1.f)},
    {vec3f( 1.f,  1.f, 1.f), vec3f(0.f, 0.f, 1.f)},

    {vec3f(1.f, -1.f,  1.f), vec3f(1.f, 0.f, 0.f)},
    {vec3f(1.f,  1.f,  1.f), vec3f(1.f, 0.f, 0.f)},
    {vec3f(1.f, -1.f, -1.f), vec3f(1.f, 0.f, 0.f)},
    {vec3f(1.f,  1.f, -1.f), vec3f(1.f, 0.f, 0.f)},

    {vec3f(-1.f, -1.f, -1.f), vec3f(-1.f, 0.f, 0.f)},
    {vec3f(-1.f,  1.f, -1.f), vec3f(-1.f, 0.f, 0.f)},
    {vec3f(-1.f, -1.f,  1.f), vec3f(-1.f, 0.f, 0.f)},
    {vec3f(-1.f,  1.f,  1.f), vec3f(-1.f, 0.f, 0.f)},

    {vec3f(-1.f, 1.f,  1.f), vec3f(0.f, 1.f, 0.f)},
    {vec3f(-1.f, 1.f, -1.f), vec3f(0.f, 1.f, 0.f)},
    {vec3f( 1.f, 1.f,  1.f), vec3f(0.f, 1.f, 0.f)},
    {vec3f( 1.f, 1.f, -1.f), vec3f(0.f, 1.f, 0.f)},

    {vec3f( 1.f, -1.f,  1.f), vec3f(0.f, -1.f, 0.f)},
    {vec3f( 1.f, -1.f, -1.f), vec3f(0.f, -1.f, 0.f)},
    {vec3f(-1.f, -1.f,  1.f), vec3f(0.f, -1.f, 0.f)},
    {vec3f(-1.f, -1.f, -1.f), vec3f(0.f, -1.f, 0.f)},
  };

  ooo::Mesh::draw_info_t di;
  di.primitive_type = render::Batch::Geometry::TriangleList;
  di.first = 0;
  di.count = sizeof(indices) / sizeof(*indices);
  di.index_type = render::Batch::Index::U16;
  di.indices_buffer = new render::Buffer(render::Buffer::Binding::Index);
  di.indices_buffer->load(indices, sizeof(indices));
  ooo::Mesh *mesh = new ooo::Mesh(di);

  ooo::Mesh::attrib_t attr_vtx;
  attr_vtx.buffer = new render::Buffer(render::Buffer::Binding::Array);
  attr_vtx.buffer->load(vertices, sizeof(vertices));
  attr_vtx.components = 3;
  attr_vtx.normalize = render::Batch::Unnormalized;
  attr_vtx.offset = offsetof(vtx_t, vtx);
  attr_vtx.stride = sizeof(vtx_t);
  attr_vtx.type = render::Batch::AttribType::Float;
  mesh->set_attribute(ooo::tag_t("VRTX"), attr_vtx);
  attr_vtx.offset = offsetof(vtx_t, nor);
  mesh->set_attribute(ooo::tag_t("NORM"), attr_vtx);

  static const char shader_vtx[] = 
    "uniform mat4 um4_mvp;\n"
    "attribute vec4 av4_vertex, av4_normal;\n"
    "varying vec3 vv3_normal;\n"
    "void main() {\n"
      "gl_Position = um4_mvp * av4_vertex;\n"
      "vv3_normal = av4_normal.xyz;\n"
    "}\n";

  static const char shader_frg[] = 
    "varying vec3 vv3_normal;\n"
    "void main() {\n"
      "gl_FragColor = vec4((vv3_normal + abs(vv3_normal))*.5, 1.);\n"
    "}\n";

  render::shader_t vertex_shader(shader_vtx, render::shader_t::type_e::vertex),
    fragment_shader(shader_frg, render::shader_t::type_e::fragment);
  render::Program *program = new render::Program(vertex_shader, fragment_shader);
  render::Material *rmaterial = new render::Material(program);
  rmaterial->depth().mode = render::depth_t::Enabled;
  rmaterial->cull().mode = render::cull_t::Enabled;
  ooo::Material *material = new ooo::Material(rmaterial);
  material->bind_attribute(ooo::tag_t("VRTX"), "av4_vertex");
  material->bind_attribute(ooo::tag_t("NORM"), "av4_normal");
  material->bind_uniform(ooo::tag_t("MMVP"), "um4_mvp");

  visual = new ooo::Visual(mesh, material);
}

void Viewport::resize(vec2i size) {
  glViewport(0, 0, size.x, size.y);
  camera_.update_aspect(size);
}

void Viewport::draw(int ms, float dt) {
  camctl_.frame(dt, ctrl_);
  //camera_.rotate_head(dt);
  //camera_.rotate_pitch(dt);
  //camera_.rotate_roll(dt);
  camera_.calc_matrix();

  GL_ASSERT

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL_ASSERT

  ooo::Visual::drawing_context_t dctx;
  dctx.camera = &camera_;
  ooo::Material::value_t &pmvp = dctx.uniforms.set(ooo::tag_t("MMVP"));
  pmvp.type = ooo::Material::value_t::Mat4;
  memcpy(pmvp.data.fp, camera_.matrix().tptr(), sizeof(mat4f));
  visual->draw(dctx);
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
    p_.window_title = "kapusha test: visuals";
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
