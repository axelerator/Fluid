//
// C++ Interface: matrixviseffect
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MATRIXVISEFFECT_H
#define MATRIXVISEFFECT_H

#include <effect.h>

/**
Just a simple esffect to visualize the current state of the matrix

	@author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class MatrixVisEffect : public Effect {
public:
    MatrixVisEffect();
    virtual void init();
    virtual void draw();
    virtual void animate(int t);
    ~MatrixVisEffect();

    private:
      float xd,yd;
};

#endif
