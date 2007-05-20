//
// C++ Interface: lineeffect
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LINEEFFECT_H
#define LINEEFFECT_H

#include "effect.h"
#include "areadetector.h"

/**
 * A simple effect detecting (possibly moving) areas and connecting them with lines.
 */
class LineEffect : public Effect {
  public:
    LineEffect(EffectSettings *conf);

    ~LineEffect();

    virtual void animate(int t);
    virtual void draw();

  private:
    AreaDetector detector; ///< The area detector that is used to detect covered areas.

    unsigned int width;    ///< Width of the matrix (just saved here for convenience & performance).
    unsigned int height;   ///< Height of the matrix (just saved here for convenience & performance).
};

#endif
