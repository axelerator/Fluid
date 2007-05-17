//
// C++ Interface: changeeffect
//
// Description:
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CHANGEEFFECT_H
#define CHANGEEFFECT_H

#include <GL/gl.h>

#include "effect.h"
#include "changedetector.h"

/**
  @author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class ChangeEffect : public Effect {
  public:
    ChangeEffect();

    ~ChangeEffect();

    virtual void animate(int t);
    virtual void draw();

  private:
    ChangeDetector detector;

    GLfloat *vertexArray;
    GLfloat *colorArray;

    unsigned int count;
    unsigned int width;
    unsigned int height;
};

#endif
