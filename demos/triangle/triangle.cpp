#include <kapusha/app.h>
#include <kapusha/viewport.h>
#include <kapusha/render.h>

using namespace kapusha;

class Viewport : public IViewport {
public:
  Viewport(IViewportController* controller);
  virtual ~Viewport() {}
  virtual void resize(vec2i);
  virtual void draw(int ms, float dt);
private:
  IViewportController *controller_;
  SBatch triangleBatch_;
}; // class Viewport

Viewport::Viewport(IViewportController* controller) : controller_(controller) {
  static const char *vertex_shader =
    "attribute vec4 vertex;\n"
    "void main() { gl_Position = vertex; }\n";
  static const char *fragment_shader =
    "void main() { gl_FragColor = vec4(.2, 1., .1, 1.); }\n";
  static vec2f vertices[3] = {
    vec2f(1.f, -1.f), vec2f(0.f, 1.f), vec2f(-1.f, -1.f)
  };

  Program *prog = new Program(vertex_shader, fragment_shader);
  Buffer *buf = new Buffer(Buffer::BindingArray);
  buf->load(vertices, sizeof(vertices), Buffer::StaticDraw);
  triangleBatch_.reset(new Batch());
  triangleBatch_->setMaterial(new Material(prog));
  triangleBatch_->setAttribSource("vertex", buf, 2, 0, sizeof(vec2f));
  triangleBatch_->setGeometry(Batch::GeometryTriangleList, 0, 3);
}

void Viewport::resize(vec2i size) {
  glViewport(0, 0, size.x, size.y);
}

void Viewport::draw(int ms, float dt) {
  glClear(GL_COLOR_BUFFER_BIT);
  triangleBatch_->draw();
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

ViewportFactory viewport_factory;

namespace kapusha {
  Application the_application = {
    &viewport_factory
  };
} // namespace kapusha
