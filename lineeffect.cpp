//
// C++ Implementation: lineeffect
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "lineeffect.h"

#include <GL/gl.h>

LineEffect::LineEffect(EffectSettings *conf) : Effect() {
  width = env->getMatrixWidth();
  height = env->getMatrixHeight();

  glLineWidth(conf->getFloat("linewidth"));
  glColor3f(conf->getFloat("colr"), conf->getFloat("colg"), conf->getFloat("colb"));
}

LineEffect::~LineEffect() {
}

void LineEffect::animate(int t) {
  detector.update();
}

void LineEffect::draw() {
  glClear ( GL_COLOR_BUFFER_BIT );

  const AreaObjects &objects = detector.getObjects();

  float xd = 4.0 / width;
  float yd = 2.0 / height;

  // Draw all covered areas.
  glBegin(GL_QUADS);
  for(unsigned int i = 0; i < objects.size(); ++i) {
    glVertex2f((objects[i].x - objects[i].width / 2) * xd - 2.0, (objects[i].y - objects[i].height / 2) * yd - 1.0);
    glVertex2f((objects[i].x + objects[i].width / 2) * xd - 2.0, (objects[i].y - objects[i].height / 2) * yd - 1.0);
    glVertex2f((objects[i].x + objects[i].width / 2) * xd - 2.0, (objects[i].y + objects[i].height / 2) * yd - 1.0);
    glVertex2f((objects[i].x - objects[i].width / 2) * xd - 2.0, (objects[i].y + objects[i].height / 2) * yd - 1.0);
  }
  glEnd();

  // Connect all areas by lines.
  glBegin(GL_LINES);
  for(unsigned int i = 0; i < objects.size(); ++i) {
    for(unsigned int j = 0; j < objects.size(); ++j) {
      glVertex2f(objects[i].x * xd - 2.0, objects[i].y * yd - 1.0);
      glVertex2f(objects[j].x * xd - 2.0, objects[j].y * yd - 1.0);
    }
  }
  glEnd();
}

