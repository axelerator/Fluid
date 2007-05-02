//
// C++ Implementation: simpleeffect
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "simpleeffect.h"

SimpleEffect::SimpleEffect() : x(0.0) {
}


SimpleEffect::~SimpleEffect() {
}

void SimpleEffect::draw() {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  gluLookAt(0.0, 0.0, 1.0,  0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  glTranslatef(sin(x), 0.0, 0.0);

  glBegin (GL_POLYGON);
  {
    glColor3f (1.0f, 0.0f, 0.0f);
    glVertex2f (0.25f, 0.5f);

    glColor3f (0.0f, 1.0f, 0.0f);
    glVertex2f (-0.25f, 0.5f);

    glColor3f (0.0f, 0.0f, 1.0f);
    glVertex2f (-0.25f, -0.5f);

    glColor3f (1.0f, 1.0f, 1.0f);
    glVertex2f (0.25f, -0.5f);
  }
  glEnd ();
}

void SimpleEffect::animate(int t) {
  x += t/1000.0;
}

