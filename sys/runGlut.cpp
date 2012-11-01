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
  
  class GlutSystem : public ISystem {
  public:
    virtual void quit(int code) {
      exit(code);
    }
    virtual void redraw() {
      redraw_requested = true;
    }
  };
  
  static GlutSystem system;

  static void display()
  {
    redraw_requested = false;
    viewport->draw(glutGet(GLUT_ELAPSED_TIME));
    glutSwapBuffers();
    if (redraw_requested)
      glutPostRedisplay();
  }

  static void resize(int w, int h)
  {
    if (!initialized)
    {
      viewport->init(&system);
      initialized = true;
    }
    viewport->resize(w, h);
  }

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
    
    glutMainLoop();
    
    delete viewport;
    viewport = 0;
    return 0;
  } // runGlut
} // namespace kapusha