//
// C++ Interface: changedetector
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CHANGEDETECTOR_H
#define CHANGEDETECTOR_H

#include <effect.h>
#include <GL/gl.h>

/**
	@author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class ChangeDetector : public Effect {
public:
    ChangeDetector();

    ~ChangeDetector();

    virtual void animate(int t);
    virtual void draw();
    
  protected:
    bool *lastMatrix;
    bool *activeMatrix;
    GLfloat *vertexArray;
    GLfloat *velocityArray;
    GLfloat *colorArray;
    std::size_t count;
    std::size_t searchRadius;
};

#endif
