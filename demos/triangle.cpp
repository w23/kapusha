// kapusha
// (c) 2012-2013 Ivan 'w23' Avdeev, me@w23.ru
#include <kapusha/viewport.h>
#include <kapusha/render.h>

using namespace kapusha;
class Viewport : public IViewport {
public:
  virtual void init(IViewportController* controller);
  virtual void resize(vec2i);
  virtual void draw(int ms, float dt);
  virtual void close();
private:
  IViewportController *controller_;
  SBatch triangleBatch_;
}; // class Viewport

void Viewport::init(IViewportController* controller) {
  static const char *vertex_shader =
    "attribute vec4 vertex;\n"
    "void main() { gl_Position = vertex; }\n";
  static const char *fragment_shader =
    "void main() { gl_FragColor = vec4(.2, 1., .1, 1.); }\n";
  static vec2f vertices[3] = {
    vec2f(1.f, -1.f), vec2f(0.f, 1.f), vec2f(-1.f, -1.f)
  };

  controller_ = controller;
  Program *prog = new Program(vertex_shader, fragment_shader);
  prog->bindAttributeLocation("vertex", 0);
  Buffer *buf = new Buffer(Buffer::BindingArray);
  buf->load(vertices, sizeof(vertices), Buffer::StaticDraw);
  triangleBatch_.reset(new Batch());
  triangleBatch_->setMaterial(new Material(prog));
  triangleBatch_->setAttribSource(0, buf, 2, 0, sizeof(vec2f));
  triangleBatch_->setGeometry(Batch::GeometryTriangleList, 0, 3);
}

void Viewport::resize(vec2i size) {
  glViewport(0, 0, size.x, size.y);
}

void Viewport::draw(int ms, float dt) {
  glClear(GL_COLOR_BUFFER_BIT);
  triangleBatch_->draw();
}

void Viewport::close() {
  triangleBatch_.reset();
}

kapusha::IViewport *makeViewport() {
  return new Viewport();
}
