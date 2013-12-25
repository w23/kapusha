#include <kapusha/viewport.h>
#include <kapusha/render.h>
#include <kapusha/ooo.h>
#include <kapusha/utils/SpectatorCameraController.h>

using namespace kapusha;

class Object {
public:
  void setBatch(Batch *batch, const char *mvp_uniform_name = "um4_mvp") {
    batch_ = batch;
    loc_mvp_ = batch_->material()->get_uniform_location(mvp_uniform_name);
  }
  void draw(const mat4f &mvp) {
    batch_->uniform_state().set_uniform(loc_mvp_, mvp);
    batch_->draw();
  }
private:
  SBatch batch_;
  int loc_mvp_;
};

class Ground : public Object {
public:
  Ground(float size);
};

class Dust : public Object {
public:
  Dust(int count, float size, float radius);
};

class Viewport : public IViewport {
public:
  Viewport(IViewportController *controller);
  virtual ~Viewport() {}
  virtual void resize(vec2i size);
  virtual void in_pointers(const Pointers& pointers);
  virtual void in_keys(const Keys& keys);
  virtual void draw(int ms, float dt);
  
private:
  IViewportController *ctrl_;
  Camera camera_;
  SpectatorCameraController camctl_;

  Ground ground_;
  Dust dust_;
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
  mat->set_uniform("us2_floor", sampler);
  Batch* batch = new Batch();
  batch->set_material(mat);
  vec3f rect[4] = {
    vec3f(-size, 0.f, -size),
    vec3f(-size, 0.f,  size),
    vec3f( size, 0.f,  size),
    vec3f( size, 0.f, -size)
  };
  Buffer *fsrect = new Buffer();
  fsrect->load(rect, sizeof rect);
  batch->set_attrib_source("av4_vtx", fsrect, 3);
  batch->set_geometry(Batch::Geometry::TriangleFan, 0, 4);
  setBatch(batch);
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
  batch->set_material(new Material(prog));
  vec3f *vertices = new vec3f[count], *p = vertices;
  for (int i = 0; i < count; ++i, ++p)
    *p = vec3f(frand(-radius, radius),
		     frand(0, radius),
		     frand(-radius, radius));
  Buffer *fsrect = new Buffer();
  fsrect->load(vertices, sizeof(*vertices) * count);
  delete vertices;
  batch->set_attrib_source("vtx", fsrect, 3, 0, sizeof(vec3f));
  batch->set_geometry(Batch::Geometry::Points, 0, count);
  setBatch(batch);
}

///////////////////////////////////////////////////////////////////////////////
Viewport::Viewport(IViewportController *controller)
 : ctrl_(controller), camctl_(camera_), dust_(8192, .1f, 20.f), ground_(20.f) {
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
  ground_.draw(camera_.getViewProjection());
  dust_.draw(camera_.getViewProjection());
}

void Viewport::in_keys(const Keys &keys) {
  if (keys.is_key_pressed(Keys::KeyEsc))
    ctrl_->quit(0);
}

void Viewport::in_pointers(const Pointers& pointers) {
  camctl_.pointers(pointers);
  if (pointers.main().was_pressed(Pointers::Pointer::LeftButton)) {
    ctrl_->grab_input(true);
    //ctrl_->hideCursor(true);
    camctl_.enable_orientation(true);
  }
  if (pointers.main().was_released(Pointers::Pointer::LeftButton)) {
    ctrl_->grab_input(false);
    //ctrl_->hideCursor(false);
    camctl_.enable_orientation(false);
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

namespace kapusha {
  const IViewportFactory *create_factory() {
    static const ViewportFactory factory;
    return &factory;
  }
} // namespace kapusha
