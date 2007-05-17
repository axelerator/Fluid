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
 * Simple detection of changes in the matrix. Changes are located
 * and have an velocity associated according to the speed of the changes.
 * 
 * Usage: Just have a private member of type ChangeDetector in your effect.
 * Call update() somewhere in the animate() method. Use isActive() to locate
 * activity and getVelocity() to get the force of the changes at a certain position.
 */
class ChangeDetector {
  public:
    ChangeDetector();

    ~ChangeDetector();

    bool isActive(unsigned int x, unsigned int y);
    const float* getVelocity(unsigned int x, unsigned int y);

    void update();

  private:
    unsigned int count;    ///< Number of elements in the matrix (just saved here for convenience & performance).
    unsigned int width;    ///< Width of the matrix (just saved here for convenience & performance).
    unsigned int height;   ///< Height the matrix (just saved here for convenience & performance).

    Environment *env;      ///< The environment (needed here because the ChangeDetector is not inherited from Effect).

    bool *lastMatrix;      ///< The matrix that was present during the previous call to update().
    bool *activeMatrix;    ///< Matrix containing the detected activities.
    float *velocityArray;  ///< Matrix containing the detected velocities (force and direction of changes).

    unsigned int searchRadius;   ///< Area that is searched for changes.
};

#endif
