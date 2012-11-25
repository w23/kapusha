#include <iostream>

#ifdef __APPLE__
#include <GLUT/GLUT.h>
#else
#include <GL/glut.h>
#endif

#include "System.h"
#include "Log.h"
#include "runGlut.h"

namespace kapusha {

#define WIDTH 640
#define HEIGHT 480

  class Syslog : public Log::ISystemLog {
  public:
    void write(const char* msg) {
      std::cerr << msg << std::endl;
    }
  };
  
  static bool initialized;
  static IViewport *viewport = 0;
  static bool redraw_requested;
  static math::vec2i center;
  static bool warped = false;
  
  class GlutSystem : public ISystem {
  public:
    virtual void quit(int code) {
      exit(code);
    }
    virtual void redraw() {
      redraw_requested = true;
    }
    virtual void pointerReset() {
      warped = true;
      glutWarpPointer(center.x, center.y);
    }
  };
  
  static GlutSystem system;
  static int last_draw;

  static void display()
  {
    redraw_requested = false;
    int now = glutGet(GLUT_ELAPSED_TIME);
    viewport->draw(now, (now - last_draw) / 1000.f);
    glutSwapBuffers();
    if (redraw_requested)
      glutPostRedisplay();
    last_draw = now;
  }

  static void resize(int w, int h)
  {
    center = math::vec2i(w/2, h/2);
    if (!initialized)
    {
      viewport->init(&system);
      initialized = true;
    }
    viewport->resize(w, h);
  }
  
  static void keyDown(unsigned char key, int, int)
  {
    viewport->userEvent(IViewport::KeyEvent(key, 0));
  }
  
  static void keyUp(unsigned char key, int, int)
  {
    viewport->userEvent(IViewport::KeyEvent(key, 0, false));
  }
  
  static void keySpecialDown(int key, int, int)
  {
    int code;
    switch (key) {
      case GLUT_KEY_LEFT:
      case GLUT_KEY_UP:
      case GLUT_KEY_RIGHT:
      case GLUT_KEY_DOWN:
        code = IViewport::KeyEvent::KeyLeft + key - GLUT_KEY_LEFT;
        break;
        
      default:
        L("Unknown special key %d", key);
    }
    viewport->userEvent(IViewport::KeyEvent(code, 0));
  }
  
  static void keySpecialUp(int key, int, int)
  {
    int code;
    switch (key) {
      case GLUT_KEY_LEFT:
      case GLUT_KEY_UP:
      case GLUT_KEY_RIGHT:
      case GLUT_KEY_DOWN:
        code = IViewport::KeyEvent::KeyLeft + key - GLUT_KEY_LEFT;
        break;
        
      default:
        L("Unknown special key %d", key);
    }

    viewport->userEvent(IViewport::KeyEvent(code, 0, false));
  }
  
  static void mouse(int button, int state, int x, int y)
  {
  }

  static void mouse_move(int x, int y)
  {
    if (warped) { warped = false; return; }
    viewport->userEvent(IViewport::PointerEvent(math::vec2f(x,y),
                                                IViewport::PointerEvent::Pointer::Move));
  }
  
////////////////////////////////////////////////////////////////////////////////

  int runGlut(int argc, const char * argv[], IViewport *vp)
  {
    KP_ASSERT(!viewport);
    viewport = vp;
    initialized = false;
    
    KP_LOG_OPEN("kapusha.log", new Syslog);
    glutInit(&argc, (char**)argv);
    
    glutInitDisplayMode(GLUT_MULTISAMPLE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Kapusha");
    
    glEnable(GL_MULTISAMPLE);
    
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(keySpecialDown);
    glutSpecialUpFunc(keySpecialUp);
    glutMouseFunc(mouse);
    glutMotionFunc(mouse_move);
    glutPassiveMotionFunc(mouse_move);
    
    glutIgnoreKeyRepeat(1);
    
    last_draw = glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop();
    
    delete viewport;
    viewport = 0;
    return 0;
  } // runGlut
} // namespace kapusha
