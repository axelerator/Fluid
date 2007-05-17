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

#include "environment.h"

/**
	@author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class ChangeDetector {
  public:
    ChangeDetector();

    ~ChangeDetector();

    bool isActive(unsigned int x, unsigned int y);
    const float* getVelocity(unsigned int x, unsigned int y);

    void update();

  private:
    unsigned int count;
    unsigned int width;
    unsigned int height;

    Environment *env;

    bool *lastMatrix;
    bool *activeMatrix;

    float *velocityArray;

    unsigned int searchRadius;
};

#endif
