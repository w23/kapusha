#include <kapusha/viewport.h>
#include <kapusha/render.h>
#include <kapusha/ooo.h>
#include <kapusha/utils/SpectatorCameraController.h>

// this source is almost a direct copy from stuff i did at
// https://www.shadertoy.com/view/MdfGW8
// all comments are stripped FOR CLARITY lols
static const char *fragmentShaderPathTracer =
"#define KTEMPBLUR .123\n"
"#define SAMPLES 16\n"
"#define REFLECTIONS 4\n"
"#define SCENE_RADIUS 100.\n"
"#define INFINITY 31337.\n"
"#define ISEC_EPSILON .01\n"
"#define REFLECT_EPSILON .01\n"
"uniform float t;\n"
"uniform vec3 uv3_pos;\n"
"varying vec3 vv3_dir;\n"
"struct ray_t { vec3 origin, dir; };\n"
"struct material_t {\n"
"  vec3 color, emission;\n"
"  float specular;\n"
"};\n"
"struct plane_t {\n"
"  vec3 normal;\n"
"  float offset;\n"
"  material_t material;\n"
"};\n"
"struct sphere_t {\n"
"  vec3 center;\n"
"  float radius2;\n"
"  material_t material;\n"
"};\n"
"plane_t planes[6];\n"
"sphere_t spheres[2];\n"
"void init_stuff() {\n"
"  planes[0] = plane_t(vec3( 1.,0.,0.), 1.,material_t(vec3(1.,.5,.2),vec3(0.),.2));\n"
"  planes[1] = plane_t(vec3(-1.,0.,0.), 1.,material_t(vec3(.5,1.,.2),vec3(0.),.2));\n"
"  planes[2] = plane_t(vec3(0., 1.,0.), 1.,material_t(vec3(.2,.5,1.),vec3(0.),.2));\n"
"  planes[3] = plane_t(vec3(0.,-1.,0.), 1.,material_t(vec3(1.,1.,1.),vec3(1.),.2));\n"
"  planes[4] = plane_t(vec3(0.,0., 1.), 1.,material_t(vec3(1.,1.,1.),vec3(0.),.9));\n"
"  planes[5] = plane_t(vec3(0.,0.,-1.), 1.,material_t(vec3(1.,1.,1.),vec3(0.),.2));\n"
"  spheres[0] = sphere_t(vec3(.3*sin(t),-.1, 0.),.04,material_t(vec3(1.),vec3(.0),1.));\n"
"  spheres[1] = sphere_t(vec3(.8*cos(t*1.7), .9*cos(t*.4), .8*sin(t)),.01,material_t(vec3(.2),vec3(.8),.3));\n"
"}\n"
"float hash(float x) { return fract(sin(x)*265871.1723); }\n"
"float isec_plane(in ray_t ray, in plane_t plane) {\n"
"  float ND = -dot(plane.normal, ray.dir);\n"
"  if (ND < ISEC_EPSILON) return INFINITY;\n"
"  float t = (dot(plane.normal, ray.origin) + plane.offset) / ND;\n"
"  if (t < 0.) return INFINITY;\n"
"  return t;\n"
"}\n"
"float isec_sphere(in ray_t ray, in sphere_t sphere) {\n"
"  vec3 v = sphere.center - ray.origin;\n"
"  float b = dot(v, ray.dir);\n"
"  float c = dot(v, v) - sphere.radius2;\n"
"  float det2 = b * b - c;\n"
"  if (det2 < 0.) return INFINITY;\n"
"  float t = b - sqrt(det2);\n"
"  if (t < 0.) return INFINITY;\n"
"  return t;\n"
"}\n"
"void main(void) {\n"
"  init_stuff();\n"
"  ray_t oray = ray_t(uv3_pos, normalize(vv3_dir));\n"
"  vec3 sumcolor = vec3(0.);\n"
"  float seed = float(t)*.24 + gl_FragCoord.x + gl_FragCoord.y;\n"
"  for (int sample = 0; sample < SAMPLES; ++sample) {\n"
"    seed += float(sample);\n"
"    ray_t ray = oray;\n"
"    vec3 kcolor = vec3(1.);\n"
"    for (int i = 0; i < REFLECTIONS; ++i) {\n"
"      float cp = INFINITY, np;\n"
"      vec3 n = vec3(0.);\n"
"      material_t cm, nm;\n"
"      for (int j = 0; j < 6; ++j) {\n"
"        np = isec_plane(ray, planes[j]);\n"
"        if (np < cp) {\n"
"          cp = np;\n"
"          cm = planes[j].material;\n"
"          n = planes[j].normal;\n"
"         }\n"
"      }\n"
"#define TR(l) (ray.origin + l * ray.dir)\n"
"      vec3 newpos = TR(cp);\n"
"      for (int j = 0; j < 2; ++j) {\n"
"        np = isec_sphere(ray, spheres[j]);\n"
"        if (np < cp) {\n"
"          cp = np;\n"
"          cm = spheres[j].material;\n"
"          newpos = TR(cp);\n"
"          n = normalize(newpos - spheres[j].center);\n"
"        }\n"
"      }\n"
//"      if (cp > SCENE_RADIUS) break;"
"      sumcolor += kcolor * cm.emission;\n"
"      kcolor *= cm.color;\n"
"      vec3 nvec = normalize(vec3(hash(seed+=newpos.x),\n"
"                                 hash(seed+=newpos.y),\n"
"                                 hash(seed+=newpos.z))*2. - vec3(1.));\n"
"      nvec *= dot(nvec, n);\n"
"      ray.dir = mix(nvec, reflect(ray.dir, n), cm.specular);\n"
"      ray.origin = newpos + n * REFLECT_EPSILON;\n"
"    }\n"
"  }\n"
"  gl_FragColor = vec4(sumcolor / float(SAMPLES), KTEMPBLUR);\n"
"}\n";

using namespace kapusha;
class Viewport : public IViewport {
public:
  Viewport() : camctl_(camera_) {}
  virtual ~Viewport() {}
  virtual void init(IViewportController *ctrl, Context *context);
  virtual void resize(vec2i);
  virtual void draw(int ms, float dt);
  void inputPointer(const PointerState &pointers);
  void inputKey(const KeyState &keys);
  virtual void close();
private:
  Context *context_;
  Camera camera_;
  SpectatorCameraController camctl_;
  IViewportController *ctrl_;
  Batch *batch_;
  Batch *output_;
  Sampler *sampler_;
  Framebuffer *fb_;
};
void Viewport::init(IViewportController *ctrl, Context *context) {
  context_ = context;
  ctrl_ = ctrl;
  static const char* svtx =
  "uniform vec2 aspect;\n"
  "uniform vec3 uv3_forward;\n"
  "attribute vec4 vtx;\n"
  "varying vec3 vv3_dir;\n"
  "void main(){\n"
  "gl_Position = vtx;\n"
  "vec3 uv = vec3(vtx.xy * aspect, 2.);\n"
  "vec3 r = cross(uv3_forward, vec3(0.,1.,0.));\n"
  "vec3 u = cross(r, uv3_forward);\n"
  "vv3_dir = uv.x * r + uv.y * u + uv.z * uv3_forward;\n"
  "}";
  vec2f rect[4] = {
    vec2f(-1.f, -1.f),
    vec2f(-1.f,  1.f),
    vec2f( 1.f,  1.f),
    vec2f( 1.f, -1.f)
  };
  Buffer *fsrect = new Buffer();
  fsrect->load(context_, rect, sizeof rect);
  Program *prog = new Program(svtx, fragmentShaderPathTracer);
  //prog->bindAttributeLocation("vtx", 0);
  batch_ = new Batch();
  Material *mat = new Material(prog);
  mat->blend().enable();
  mat->blend().setFunction(BlendState::SourceAlpha, BlendState::OneMinusSourceAlpha);
  batch_->setMaterial(mat);
  batch_->setAttribSource("vtx", fsrect, 2);
  batch_->setGeometry(Batch::GeometryTriangleFan, 0, 4);
  camera_.lookAt(vec3f(.8f), vec3f(0.f));
  camctl_.setSpeed(1.f);

  sampler_ = new Sampler(Sampler::Nearest, Sampler::Nearest);
  fb_ = new Framebuffer();
  static const char *osvtx = 
    "attribute vec4 vtx;\n"
    "varying vec2 p;\n"
    "void main() {\n"
      "p = vtx.xy * .5 + .5;\n"
      "gl_Position = vtx;\n"
    "}\n";
  static const char *osfrg = 
    "uniform sampler2D frame;\n"
    "varying vec2 p;\n"
    "void main() {\n"
      "gl_FragColor = pow(texture2D(frame, p), vec4(.7));\n"
    "}\n";
  Program *oprog = new Program(osvtx, osfrg);
  output_ = new Batch();
  Material *omat = new Material(oprog);
  omat->setUniform("frame", sampler_);
  omat->blend().disable();
  output_->setMaterial(omat);
  output_->setAttribSource("vtx", fsrect, 2);
  output_->setGeometry(Batch::GeometryTriangleFan, 0, 4);
}
void Viewport::close() {
  delete batch_;
  delete fb_;
  delete output_;
  context_ = 0;
}
void Viewport::resize(vec2i size) {
  glViewport(0, 0, size.x, size.y);
  glClear(GL_COLOR_BUFFER_BIT);
  vec2f aspect((float)size.x / size.y, 1.f);
  batch_->getMaterial()->setUniform("aspect", aspect);
  sampler_->upload(context_, Sampler::Meta(size), 0);
  fb_->attachColor(context_, sampler_, 0);
  fb_->bind(context_);
  glViewport(0, 0, size.x, size.y);
  glClear(GL_COLOR_BUFFER_BIT);
}
void Viewport::draw(int ms, float dt) {
  camctl_.frame(dt, ctrl_);
  camera_.setPosition(camera_.position().clamped(vec3f(-.9f), vec3f(.9f)));
  float time = ms / 1000.f;
  camera_.update();
  batch_->getMaterial()->setUniform("t", time);
  batch_->getMaterial()->setUniform("uv3_pos", camera_.position());
  batch_->getMaterial()->setUniform("uv3_forward", camera_.forward().normalized());
  fb_->bind(context_);
  batch_->draw(context_);
  context_->bindFramebuffer(0);
  glClear(GL_COLOR_BUFFER_BIT);
  output_->draw(context_);
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
void Viewport::inputKey(const KeyState &keys) {
  if (keys.isKeyPressed(KeyState::KeyEsc)) ctrl_->quit(0);
}
IViewport *makeViewport() {
  return new Viewport;
}