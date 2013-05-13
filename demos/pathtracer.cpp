#include <kapusha/viewport.h>
#include <kapusha/render.h>
#include <kapusha/ooo.h>
#include <kapusha/utils/SpectatorCameraController.h>

//! \todo temporal blur

// this source is almost a direct copy from stuff i did at
// https://www.shadertoy.com/view/MdfGW8
// all comments are stripped FOR CLARITY lols
static const char *fragmentShaderPathTracer =
"#define SAMPLES 16\n"
"#define SCENE_RADIUS 100.\n"
"#define INFINITY 31337.\n"
"#define ISEC_EPSILON .01\n"
"#define REFLECT_EPSILON .01\n"
"uniform float t;\n"
"uniform vec3 uv3_pos;\n"
"varying vec3 vv3_dir;\n"
"struct ray_t { vec3 origin, dir; };\n"
"struct isection_t { vec3 pos, normal; float path; };\n"
"struct material_t { vec3 diffuse, emission; float specular; };\n"
"struct plane_t {\n"
"  vec3 normal; float offset;\n"
"  material_t material;\n"
"};\n"
"struct sphere_t {\n"
"  vec3 center; float radius;\n"
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
"  spheres[0] = sphere_t(vec3(.3*sin(t),-.1, 0.),.2,\n"
"                        material_t(vec3(1.),vec3(.0),1.));\n"
"  spheres[1] = sphere_t(vec3(.8*cos(t*1.7), .9*cos(t*.4), .8*sin(t)),.1,\n"
"                        material_t(vec3(.2),vec3(.8),.3));\n"
"}\n"
"isection_t make_isection(in ray_t ray, in float path, in vec3 normal) {\n"
"  return isection_t(ray.origin + ray.dir * path, normal, path);\n"
"}\n"
"isection_t isection_none() {\n"
"  return isection_t(vec3(0.), vec3(0.), INFINITY);\n"
"}\n"
"float hash(float x) {\n"
"  return fract(cos(x)*265871.1723);\n"
"}\n"
"isection_t isec_plane(in ray_t ray, in plane_t plane) {\n"
"  float ND = -dot(plane.normal, ray.dir);\n"
"  if (ND < ISEC_EPSILON) return isection_none();\n"
"  float t = (dot(plane.normal, ray.origin) + plane.offset) / ND;\n"
"  if (t <= 0.) return isection_none();\n"
"  return make_isection(ray, t, plane.normal);\n"
"}\n"
"isection_t isec_sphere(in ray_t ray, in sphere_t sphere) {\n"
"  vec3 v = sphere.center - ray.origin;\n"
"  float b = 2. * dot(v, ray.dir);\n"
"  float c = dot(v, v) - sphere.radius * sphere.radius;\n"
"  float det2 = b * b - 4. * c;\n"
"  if (det2 < 0.) return isection_none();\n"
"  float t = .5 * (b - sqrt(det2));\n"
"  if (t < 0.) return isection_none();\n"
"  vec3 p = ray.origin + ray.dir * t;\n"
"  return isection_t(p, normalize(p - sphere.center), t);\n"
"}\n"
"void main(void) {\n"
"  init_stuff();\n"
"  vec3 sumcolor = vec3(0.);\n"
"  ray_t oray = ray_t(uv3_pos, normalize(vv3_dir));\n"
"  float seed = float(t)*.24 + gl_FragCoord.x + gl_FragCoord.y;\n"
"  for (int sample = 0; sample < SAMPLES; ++sample) {\n"
"    seed += float(sample);\n"
"    ray_t ray = oray;\n"
"    material_t matstack[4];\n"
"    matstack[0] = material_t(vec3(0.), vec3(0.), 0.);\n"
"    matstack[1] = material_t(vec3(0.), vec3(0.), 0.);\n"
"    matstack[2] = material_t(vec3(0.), vec3(0.), 0.);\n"
"    matstack[3] = material_t(vec3(0.), vec3(0.), 0.);\n"
"    for (int i = 0; i < 4; ++i) {\n"
"      isection_t ni, ci = isection_none();\n"
"      material_t cm, nm;\n"
"      for (int j = 0; j < 6; ++j) {\n"
"        ni = isec_plane(ray, planes[j]);\n"
"        if (ni.path < ci.path) { ci = ni; cm = planes[j].material; }\n"
"      }\n"
"      for (int j = 0; j < 2; ++j) {\n"
"        ni = isec_sphere(ray, spheres[j]);\n"
"        if (ni.path < ci.path) { ci = ni; cm = spheres[j].material; }\n"
"      }\n"
"      //if (ci.path > SCENE_RADIUS) break;\n"
"      matstack[i] = cm;\n"
"      vec3 nvec = vec3(hash(seed+=ci.pos.x),\n"
"                       hash(seed+=ci.pos.y),\n"
"                       hash(seed+=ci.pos.z))*2. - 1.;\n"
"      nvec *= dot(nvec, ci.normal);\n"
"      ray.dir = mix(normalize(nvec), reflect(ray.dir, ci.normal), cm.specular);\n"
"      ray.origin = ci.pos + ci.normal * REFLECT_EPSILON;\n"
"    } // for all bounces\n"
"    vec3 color = matstack[3].emission;\n"
"    color = matstack[2].diffuse * color + matstack[2].emission;\n"
"    color = matstack[1].diffuse * color + matstack[1].emission;\n"
"    color = matstack[0].diffuse * color + matstack[0].emission;\n"
"    sumcolor += color;\n"
"  } // for all samples\n"
"  gl_FragColor = vec4(sumcolor / float(SAMPLES), 1.);\n"
"}\n";

using namespace kapusha;
class Viewport : public IViewport {
public:
  Viewport() : camctl_(camera_) {}
  virtual ~Viewport() {}
  virtual void init(IViewportController* ctrl);
  virtual void resize(vec2i);
  virtual void draw(int ms, float dt);
  void inputPointer(const PointerState& pointers);
  virtual void close();
private:
  Camera camera_;
  SpectatorCameraController camctl_;
  IViewportController *ctrl_;
  Batch *batch_;
};
void Viewport::init(IViewportController *ctrl) {
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
  fsrect->load(rect, sizeof rect);
  Program *prog = new Program(svtx, fragmentShaderPathTracer);
  prog->bindAttributeLocation("vtx", 0);
  batch_ = new Batch();
  batch_->setMaterial(new Material(prog));
  batch_->setAttribSource(0, fsrect, 2);
  batch_->setGeometry(Batch::GeometryTriangleFan, 0, 4);
  camera_.lookAt(vec3f(.8f), vec3f(0.f));
  camctl_.setSpeed(1.f);
}
void Viewport::close() { delete batch_; }
void Viewport::resize(vec2i size) {
  glViewport(0, 0, size.x, size.y);
  vec2f aspect((float)size.x / size.y, 1.f);
  batch_->getMaterial()->setUniform("aspect", aspect);
}
void Viewport::draw(int ms, float dt) {
  camctl_.frame(dt, ctrl_);
  camera_.setPosition(camera_.position().clamped(vec3f(-.9f), vec3f(.9f)));
  GL_ASSERT
  glClear(GL_COLOR_BUFFER_BIT);
  GL_ASSERT
  float time = ms / 1000.f;
  camera_.update();
  batch_->getMaterial()->setUniform("t", time);
  batch_->getMaterial()->setUniform("uv3_pos", camera_.position());
  batch_->getMaterial()->setUniform("uv3_forward", camera_.forward().normalized());
  batch_->draw();
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
