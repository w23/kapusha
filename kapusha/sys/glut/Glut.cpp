#include <GLUT/glut.h>
#include "Glut.h"

namespace kapusha {
  class GlutController : public IViewportController {
  public:
    GlutController(vec2i size, IViewport *viewport)
      : size_(size), viewport_(viewport), need_redraw_(true) {
        KP_ASSERT(!controller_);
        controller_ = this;
    }
    ~GlutController() { controller_ = 0; }
    int run(int argc, char *argv[]);
    virtual void quit(int code) { exit(code); }
    virtual void requestRedraw() {
      if (!need_redraw_)
        glutPostRedisplay();
      need_redraw_ = true; }
    virtual void limitlessPointer(bool limitless) {}
    virtual void hideCursor(bool hide) {}
    virtual const PointerState& pointerState() const { return pointers_; }
    virtual const KeyState& keyState() const { return keys_; }
  private:
    static void displayFunc() { controller_->display(); }
    static void reshapeFunc(int w, int h) { controller_->reshape(w, h); }
    static void keyboardFunc(unsigned char key, int x, int y) {}
    static void mouseFunc(int button, int state, int x, int y) {}
    static void motionFunc(int x, int y) { controller_->motion(x, y); }
    void display();
    void reshape(int w, int h);
    void motion(int x, int y);
    static GlutController *controller_;
    vec2i size_;
    IViewport *viewport_;
    KeyState keys_;
    PointerState pointers_;
    int lastframetime_;
    bool need_redraw_;
  };
  
  GlutController *GlutController::controller_ = 0;

  int GlutController::run(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(size_.x, size_.y);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("kapusha");
    glutDisplayFunc(displayFunc);
    glutReshapeFunc(reshapeFunc);
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);
    glutPassiveMotionFunc(motionFunc);
    //glutWMCloseFunc(close_cb);
    viewport_->init(this);
    lastframetime_ = glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop();
    return 0;
  }
  
  void GlutController::display() {
    need_redraw_ = false;
    int now = glutGet(GLUT_ELAPSED_TIME);
    viewport_->draw(now, (now - lastframetime_) / 1000.f);
    glutSwapBuffers();
    lastframetime_ = now;
    if (need_redraw_)
      glutPostRedisplay();
  }
  
  void GlutController::reshape(int w, int h) {
    viewport_->resize(size_ = vec2i(w, h));
  }
  
  void GlutController::motion(int x, int y) {
    pointers_.mouseMove(vec2f(x, y) / size_, glutGet(GLUT_ELAPSED_TIME));
    viewport_->inputPointer(pointers_);
  }
  
  int runGlut(int argc, char *argv[], vec2i size, IViewport *viewport) {
    GlutController controller(size, viewport);
    return controller.run(argc, argv);
  }
} // namespace kapusha