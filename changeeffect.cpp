//
// C++ Implementation: changeeffect
//
// Description:
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "changeeffect.h"

ChangeEffect::ChangeEffect() : Effect() {
  width = env->getMatrixWidth();
  height = env->getMatrixHeight();
  count = width * height;

  vertexArray = new GLfloat[2 * count];
  colorArray  = new GLfloat[3 * count];

  float xd = 4.0 / width;
  float yd = 2.0 / height;

  for (unsigned int i = 0; i < height; ++i) {
    for (unsigned int t = 0; t < width; ++t) {
      int vidx = (i * width + t) * 2;
      vertexArray[vidx] = t * xd - 2.0 ;
      vertexArray[vidx+1] =  i * yd - 1.0 ;

      int cidx = (i * width + t) * 3;
      colorArray[cidx] = 0.3;
      colorArray[cidx+1] = 0.3;
      colorArray[cidx+2] = 0.3;
    }
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glVertexPointer(2, GL_FLOAT,0, vertexArray);
  glColorPointer(3, GL_FLOAT,0, colorArray);
  glPointSize(1.0);
}

ChangeEffect::~ChangeEffect() {
  delete[] vertexArray;
  delete[] colorArray;
}

void ChangeEffect::animate(int t) {
  detector.update();

  for (unsigned int x = 0; x < width; ++x) {
    for (unsigned int y = 0; y < height; ++y) {
      int cidx = (x + y * width) * 3;

      if (detector.isActive(x, y)) {
        colorArray[cidx] = 1.0;
        colorArray[cidx+1] = 1.0;
        colorArray[cidx+2] = 1.0;
      }
      else {
        colorArray[cidx] = 0.3;
        colorArray[cidx+1] = 0.3;
        colorArray[cidx+2] = 0.3;
      }
    }
  }
}

void ChangeEffect::draw() {
  glClear ( GL_COLOR_BUFFER_BIT );
  glMatrixMode ( GL_MODELVIEW );
  glLoadIdentity ();
  glDrawArrays ( GL_POINTS, 0, count );

  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  for (unsigned int x = 0; x < width; ++x) {
    for (unsigned int y = 0; y < height; ++y) {
      if (detector.isActive(x, y)) {
        unsigned int vertex = (x + y * width) * 2;
        const float* vel = detector.getVelocity(x, y);

        glVertex2fv(vertexArray + vertex);
        glVertex2f(vertexArray[vertex] + vel[0], vertexArray[vertex+1] + vel[1]);
      }
    }
  }
  glEnd();
}

