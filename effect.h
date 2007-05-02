//
// C++ Interface: effect
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef EFFECT_H
#define EFFECT_H

#include "environment.h"

/**
  @author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class Effect {
  public:
    Effect() {
      env = Environment::getInstance();
    };
    virtual ~Effect() {};
    virtual void draw() = 0;
    virtual void animate(int t) = 0;

  private:
    const Environment* env;
};

#endif
