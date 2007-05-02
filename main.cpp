//
// C++ Implementation: main
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <iostream>
#include <string>

#include <GL/glut.h>

#include "effectmanager.h"
#include "environment.h"

void display() {
  EffectManager::getInstance()->draw();

  glutSwapBuffers();
}

void timer(int lastCallTime) {
  int thisCallTime = glutGet(GLUT_ELAPSED_TIME);

  int interval = thisCallTime - lastCallTime;

  EffectManager *mgr = EffectManager::getInstance();
  mgr->animate(interval);

  glutTimerFunc(1000 / mgr->getEnvironment()->getFps(), timer, thisCallTime);

  glutPostRedisplay();
}

bool initOpenGL(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glShadeModel(GL_SMOOTH);     /* fuer Smooth-Shading */
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glColor3f(1.0f, 1.0f, 1.0f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, 2.6667, 1.0, 100.0);

  return (glGetError() == GL_NO_ERROR);
}

int createWindow(std::string title, int width, int height) {
  int windowID = 0;

  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (width, height);
  glutInitWindowPosition (0, 0);

  windowID = glutCreateWindow (title.c_str());

  return windowID;
}

int main(int argc, char *argv[]) {
  std::cout << "Initializing Effect Master 3000...";

  glutInit(&argc, argv);

  Environment *env = EffectManager::getInstance()->getEnvironment();
  env->loadConfig("dummy path");

  if (!createWindow("Effect Master 3000", env->getScreenWidth(), env->getScreenHeight()) || 
      !initOpenGL(env->getScreenWidth(), env->getScreenHeight())) {
    std::cout << "error!" << std::endl;
    exit(1);
  }

  glutDisplayFunc(display);
  glutTimerFunc(1000 / env->getFps(), timer, glutGet(GLUT_ELAPSED_TIME));

  std::cout << "done!" << std::endl;

  glutMainLoop();

  return 0;
}
