//
// C++ Interface: simplesparkle
//
// Description:
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SIMPLESPARKLE_H
#define SIMPLESPARKLE_H
#include <GL/glew.h>
#include "effect.h"

class EffectSettings;

/**
	@author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class SimpleSparkle : public Effect {
public:
    SimpleSparkle();
    SimpleSparkle(EffectSettings *conf);

    ~SimpleSparkle();

    virtual void animate(int t);
    virtual void draw();

  private:
    void init(const char *texture);

    float xd,yd;
    GLuint particleTex;
    GLfloat *vertexArray;
    GLfloat *colorArray;
    GLfloat *vel;
    int count;
    GLfloat variance;
    GLfloat maxSpeed;
    GLfloat aging;
    GLfloat pointSize;
};

#endif
