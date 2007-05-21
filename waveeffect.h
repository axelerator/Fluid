//
// C++ Interface: waveeffect
//
// Description:
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef WAVEEFFECT_H
#define WAVEEFFECT_H

#include <effect.h>
#include <GL/glew.h>
#include "changedetector.h"

/**
  @author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class WaveEffect : public Effect {
  public:
    WaveEffect(EffectSettings *conf);

    ~WaveEffect();

    virtual void animate ( int t );
    virtual void draw();

  private:
    unsigned int width;
    unsigned int height;
    unsigned int count;

    GLfloat *vertexArray;
    GLfloat *colorArray;
    GLfloat *normalArray;

    unsigned int frame;

    float *input;
    float *output;
    GLuint *indices;
    
    float friction;
    GLuint waterTex;
    ChangeDetector detector;
};

#endif
