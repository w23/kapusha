#include "../sys/System.h"
#include "../sys/Log.h"
#include "../math/types.h"
#include "../sys/IViewport.h"
#include "../sys/runGlut.h"
#include "../gl/OpenGL.h"
#include "../gl/Batch.h"
#include "../gl/Object.h"
#include "../gl/Camera.h"
#include "../gl/Buffer.h"
#include "../gl/Program.h"
#include "../gl/Material.h"

using namespace kapusha;

class Viewport : public IViewport {
public:
  Viewport();
  virtual ~Viewport() {}
  virtual void init(ISystem* system);
  virtual void resize(int width, int height);
  virtual void draw(int ms, float dt);
  virtual void keyEvent(const KeyEvent& event);
  virtual void pointerEvent(const PointerEvent& event);
  
private:
  Object* createGround() const;
  Object* createDust() const;
  
private:
  ISystem *system_;
  Object* ground_;
  Object* object_;
  Camera camera_;
  
  float forward_speed_;
  float right_speed_;
  float pitch_speed_;
  float yaw_speed_;
  
  math::rect2f viewport_;
};

Viewport::Viewport()
: forward_speed_(0), right_speed_(0), pitch_speed_(0), yaw_speed_(0)
{
}

Object* Viewport::createGround() const
{
  Batch* batch = new Batch();

  const char* svtx =
  "uniform mat4 mview, mproj;\n"
  "attribute vec4 vtx;\n"
  "varying vec2 p;\n"
  "void main(){\n"
  "gl_Position = mproj * mview * vtx;\n"
  "p = vtx.xz;\n"
  "}"
  ;
  
  const char* sfrg =
  "varying vec2 p;\n"
  "void main(){\n"
    "gl_FragColor = vec4(mod(floor(p.x)+floor(p.y),2.)) * (20. - length(p)) / 20.;\n"
  "}"
  ;
  Material *mat = new Material(new Program(svtx, sfrg));
  batch->setMaterial(mat);
  
  math::vec3f rect[4] = {
    math::vec3f(-100.f, -2.f, -100.f),
    math::vec3f(-100.f, -2.f,  100.f),
    math::vec3f( 100.f, -2.f,  100.f),
    math::vec3f( 100.f, -2.f, -100.f)
  };
  Buffer *fsrect = new Buffer();
  fsrect->load(rect, sizeof rect);
  batch->setAttribSource("vtx", fsrect, 3);
  
  batch->setGeometry(Batch::GeometryTriangleFan, 0, 4, 0);
  
  return new Object(batch);
}

Object* Viewport::createDust() const
{
  Batch* batch = new Batch();
  
  const char* svtx =
  "uniform mat4 mview, mproj;\n"
  "attribute vec4 vtx;\n"
  "varying float light;\n"
  "void main(){\n"
  "gl_Position = mproj * mview * vtx;\n"
  "gl_PointSize = 10. / gl_Position.z;\n"
  "light = (20. - length(vtx)) / 20.;\n"
  "}"
  ;
  const char* sfrg =
  "varying float light;\n"
  "void main(){\n"
  "gl_FragColor = vec4(1.) * light;\n"
  "}"
  ;
  Material *mat = new Material(new Program(svtx, sfrg));
  batch->setMaterial(mat);
  
  const int particles = 8192;
  const float radius = 10.f;
  
  math::vec3f *vertices = new math::vec3f[particles], *p = vertices;
  
  for (int i = 0; i < particles; ++i, ++p)
  {
    *p = math::vec3f(math::frand(-radius, radius),
                     math::frand(-2, radius),
                     math::frand(-radius, radius));
  }
  
  Buffer *fsrect = new Buffer();
  fsrect->load(vertices, sizeof(*vertices) * particles);
  
  delete vertices;
  
  batch->setAttribSource("vtx", fsrect, 3, 0, 2*sizeof(math::vec3f));
  
  batch->setGeometry(Batch::GeometryPoints, 0, particles);
  return new Object(batch);
}

void Viewport::init(ISystem *system)
{
  system_ = system;
  
  object_ = createDust();
  ground_ = createGround();
  
  glEnable(GL_DEPTH_TEST);
  GL_ASSERT
  glEnable(GL_CULL_FACE);
  GL_ASSERT
  glEnable(GL_POINT_SPRITE);
  GL_ASSERT
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  GL_ASSERT
}

void Viewport::resize(int width, int height)
{
  glViewport(0, 0, width, height);
  viewport_ = math::rect2f(0, height, width, 0);
  camera_.setAspect((float)width / (float)height);
}

void Viewport::draw(int ms, float dt)
{
  camera_.moveForward(forward_speed_ * dt);
  camera_.moveRigth(right_speed_ * dt);
  camera_.rotatePitch(pitch_speed_ * dt);
  camera_.rotateYaw(yaw_speed_ * dt);
  camera_.update();
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ground_->draw(camera_.getView(), camera_.getProjection());
  object_->draw(camera_.getView(), camera_.getProjection());
  
  system_->redraw();
}

void Viewport::keyEvent(const kapusha::IViewport::KeyEvent &event)
{
  switch (event.key()) {
    case 'w':
      forward_speed_ += event.isPressed() ? 1.f : -1.f;
      break;
    case 's':
      forward_speed_ += event.isPressed() ? -1.f : 1.f;
      break;
    case 'a':
      right_speed_ += event.isPressed() ? -1.f : 1.f;
      break;
    case 'd':
      right_speed_ += event.isPressed() ? 1.f : -1.f;
      break;
    case KeyEvent::KeyUp:
      pitch_speed_ += event.isPressed() ? 1.f : -1.f;
      break;
    case KeyEvent::KeyDown:
      pitch_speed_ += event.isPressed() ? -1.f : 1.f;
      break;
    case KeyEvent::KeyLeft:
      yaw_speed_ += event.isPressed() ? 1.f : -1.f;
      break;
    case KeyEvent::KeyRight:
      yaw_speed_ += event.isPressed() ? -1.f : 1.f;
      break;

    default:
      L("key %d is unknown", event.key());
  }
}

void Viewport::pointerEvent(const PointerEvent &event)
{
  math::vec2f rel = viewport_.relative(event.main().point)*2.f - 1.f;
  yaw_speed_ = -rel.x;
  pitch_speed_ = rel.y;
}

int main(int argc, const char * argv[])
{
  return runGlut(argc, argv, new Viewport);
}