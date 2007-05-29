//
// C++ Interface: catchsparkseffect
//
// Description:
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CATCHSPARKSEFFECT_H
#define CATCHSPARKSEFFECT_H

#include <effect.h>
#include <GL/glu.h>
#include "areadetector.h"

class Flower;
/**
  @author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class CatchSparksEffect : public Effect
{
  public:
    CatchSparksEffect(EffectSettings *conf);

    ~CatchSparksEffect();

    virtual void animate ( int t );
    virtual void draw();
    
  private:
    unsigned int flowerCount;
    unsigned int blossomCount;
    unsigned int sparkCount;
    
    Flower *flowers;
    GLuint blossomTex;
    GLuint sparkTex;
    AreaDetector detector;
    
    float xd,yd;
    
    GLfloat *vertices;
    GLfloat *colors;
};

#endif
