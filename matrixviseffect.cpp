//
// C++ Implementation: matrixviseffect
//
// Description:
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "matrixviseffect.h"
#include <GL/gl.h>
#include "effectmanager.h"
#include "environment.h"
MatrixVisEffect::MatrixVisEffect()
: Effect() {
    xd = 4.0 / env->getMatrixWidth();
    yd = 2.0 / env->getMatrixHeight();

}


MatrixVisEffect::~MatrixVisEffect() {}

void MatrixVisEffect::init() {
  glPointSize(4.0);
  glEnable(GL_POINT_SMOOTH);
}

void MatrixVisEffect::draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    //Environment *env = EffectManager::getInstance()->getEnvironment();
    glColor3f(0.9, 0.9, 0.9);
    bool* m = (bool*)env->getMatrix();
    glBegin(GL_POINTS);
    for (int i = 0; i < env->getMatrixHeight(); ++i )
        for (int t = 0; t < env->getMatrixWidth(); ++t )
            if (m[i*env->getMatrixWidth() + t] )
                glVertex3f(t*xd - 2.0, i * yd - 1.0, 0.0);
    glEnd();
}

void MatrixVisEffect::animate(int t) {
}
