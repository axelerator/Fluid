//
// C++ Interface: simpleeffect
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SIMPLEEFFECT_H
#define SIMPLEEFFECT_H

#include <effect.h>

/**
	@author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class SimpleEffect : public Effect {
  public:
    SimpleEffect();
    virtual void draw();
    virtual void animate(int t);
    virtual ~SimpleEffect();

  private:
    double x;
};

#endif
