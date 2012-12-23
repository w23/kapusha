#include "../core/Core.h"
#include "../math/types.h"
#include "../core/IViewport.h"
#include "../render/Render.h"
#include "../render/Camera.h"

namespace flyby {

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
    Object* createGround() const;
    Object* createDust() const;
    
  private:
    IViewportController *ctrl_;
    Object* ground_;
    Object* object_;
    Camera camera_;
    
    float forward_speed_;
    float right_speed_;
    float pitch_speed_;
    float yaw_speed_;
    
    rect2f viewport_;
  };

  Viewport::Viewport()
  : forward_speed_(0), right_speed_(0), pitch_speed_(0), yaw_speed_(0)
  {
  }

  Object* Viewport::createGround() const
  {
    Batch* batch = new Batch();

    const char* svtx =
    "uniform mat4 um4_view, um4_proj;\n"
    "attribute vec4 vtx;\n"
#if KAPUSHA_GLES
    "varying mediump vec2 p;\n"
#else
    "varying vec2 p;\n"
#endif
    "void main(){\n"
    "gl_Position = um4_proj * um4_view * vtx;\n"
    "p = vtx.xz;\n"
    "}"
    ;
    
    const char* sfrg =
#if KAPUSHA_GLES
    "varying mediump vec2 p;\n"
#else
    "varying vec2 p;\n"
#endif
    "void main(){\n"
      "gl_FragColor = vec4(mod(floor(p.x)+floor(p.y),2.)) * (20. - length(p)) / 20.;\n"
    "}"
    ;
    Material *mat = new Material(new Program(svtx, sfrg));
    batch->setMaterial(mat);
    
    vec3f rect[4] = {
      vec3f(-100.f, -2.f, -100.f),
      vec3f(-100.f, -2.f,  100.f),
      vec3f( 100.f, -2.f,  100.f),
      vec3f( 100.f, -2.f, -100.f)
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
    "uniform mat4 um4_view, um4_proj;\n"
    "attribute vec4 vtx;\n"
#if KAPUSHA_GLES
    "varying lowp float light;\n"
#else
    "varying float light;\n"
#endif
    "void main(){\n"
    "gl_Position = um4_proj * um4_view * vtx;\n"
    "gl_PointSize = 10. / gl_Position.z;\n"
    "light = (20. - length(vtx)) / 20.;\n"
    "}"
    ;
    const char* sfrg =
#if KAPUSHA_GLES
    "varying lowp float light;\n"
#else
    "varying float light;\n"
#endif
    "void main(){\n"
    "gl_FragColor = vec4(1.) * light;\n"
    "}"
    ;
    Material *mat = new Material(new Program(svtx, sfrg));
    batch->setMaterial(mat);
    
    const int particles = 8192;
    const float radius = 10.f;
    
    vec3f *vertices = new vec3f[particles], *p = vertices;
    
    for (int i = 0; i < particles; ++i, ++p)
    {
      *p = vec3f(frand(-radius, radius),
                       frand(-2, radius),
                       frand(-radius, radius));
    }
    
    Buffer *fsrect = new Buffer();
    fsrect->load(vertices, sizeof(*vertices) * particles);
    
    delete vertices;
    
    batch->setAttribSource("vtx", fsrect, 3, 0, sizeof(vec3f));
    
    batch->setGeometry(Batch::GeometryPoints, 0, particles);
    return new Object(batch);
  }

  void Viewport::init(IViewportController *ctrl)
  {
    ctrl_ = ctrl;
    
    object_ = createDust();
    ground_ = createGround();
    
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

  void Viewport::close()
  {
    delete object_;
    delete ground_;
  }

  void Viewport::resize(vec2i size)
  {
    glViewport(0, 0, size.x, size.y);
    viewport_ = rect2f(0, size.y, size.x, 0);
    camera_.setAspect((float)size.x / (float)size.y);
  }

  void Viewport::draw(int ms, float dt)
  {
    camera_.moveForward(forward_speed_ * dt);
    camera_.moveRigth(right_speed_ * dt);
    camera_.rotatePitch(pitch_speed_ * dt);
    camera_.rotateYaw(yaw_speed_ * dt);
    camera_.update();
    
    GL_ASSERT
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GL_ASSERT

    ground_->draw(camera_.getView(), camera_.getProjection());
    object_->draw(camera_.getView(), camera_.getProjection());
    
    ctrl_->requestRedraw();
  }

  void Viewport::inputKey(const kapusha::KeyState &keys)
  {
    switch (keys.getLastKey()) {
      case 'W':
        forward_speed_ += keys.isLastKeyPressed() ? 1.f : -1.f;
        break;
      case 'S':
        forward_speed_ += keys.isLastKeyPressed() ? -1.f : 1.f;
        break;
      case 'A':
        right_speed_ += keys.isLastKeyPressed() ? -1.f : 1.f;
        break;
      case 'D':
        right_speed_ += keys.isLastKeyPressed() ? 1.f : -1.f;
        break;
      case KeyState::KeyUp:
        pitch_speed_ += keys.isLastKeyPressed() ? 1.f : -1.f;
        break;
      case KeyState::KeyDown:
        pitch_speed_ += keys.isLastKeyPressed() ? -1.f : 1.f;
        break;
      case KeyState::KeyLeft:
        yaw_speed_ += keys.isLastKeyPressed() ? 1.f : -1.f;
        break;
      case KeyState::KeyRight:
        yaw_speed_ += keys.isLastKeyPressed() ? -1.f : 1.f;
        break;

      default:
        L("key %d is unknown", keys.getLastKey());
    }
  }

  void Viewport::inputPointer(const kapusha::PointerState &pointers)
  {
    yaw_speed_ = -pointers.main().point.x;
    pitch_speed_ = pointers.main().point.y;
  }

  IViewport *makeViewport()
  {
    return new Viewport;
  }
} // namespace flyby

#if SAMPLE_STANDALONE_GLUT
namespace kapusha {
  int runGlut(int argc, const char* argv[], kapusha::IViewport*);
}
int main(int argc, const char* argv[])
{
  return kapusha::runGlut(argc, argv, new flyby::Viewport);
}
#endif
