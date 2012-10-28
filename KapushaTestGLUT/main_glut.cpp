#include <iostream>

#ifdef __APPLE__
#include <GLUT/GLUT.h>
#else
#include <GL/glut.h>
#endif

#include "../sys/System.h"
#include "../sys/Log.h"
#include "../math/types.h"
#include "../gl/Object.h"
#include "../gl/Buffer.h"
#include "../gl/Program.h"

using namespace kapusha;

#define WIDTH 640
#define HEIGHT 480

class Syslog : public Log::ISystemLog {
public:
  void write(const char* msg) {
    std::cerr << msg << std::endl;
  }
};

static Object* object = 0;

static void display()
{
  object->draw();
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  glViewport(0, 0, w, h);
  if (!object)
  {
    object = new Object();
    
    const char* svtx =
    "attribute vec4 vtx;\n"
    "void main(){\n"
    "gl_Position = vtx;\n"
    "}"
    ;
    const char* sfrg =
    "void main(){\n"
    "gl_FragColor = vec4(1.,0.,0.,0.);\n"
    "}"
    ;
    Program *prog = new Program(svtx, sfrg);
    object->setProgram(prog);
    
    math::vec2f rect[4] = {
      math::vec2f(-1.f, -1.f),
      math::vec2f(-1.f,  1.f),
      math::vec2f( 1.f,  1.f),
      math::vec2f( 1.f, -1.f)
    };
    Buffer *fsrect = new Buffer();
    fsrect->load(rect, sizeof rect);
    object->setAttribSource("vtx", fsrect, 2);
    
    object->setGeometry(0, 0, 4, Object::GeometryTriangleFan);
  }
}

int main(int argc, const char * argv[])
{
  KP_LOG_OPEN(0, new Syslog);
  glutInit(&argc, (char**)argv);
  
  glutInitDisplayMode(GLUT_MULTISAMPLE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("KapushaTest");
  
  glEnable(GL_MULTISAMPLE);
  
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  
  glutMainLoop();
  return 0;
}