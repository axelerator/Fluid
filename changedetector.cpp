//
// C++ Implementation: changedetector
//
// Description:
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "changedetector.h"

#include <math.h>
#include <string.h>

/**
 * Creates a changedetector.
 * 
 * \todo Make some settings configurable.
 */
ChangeDetector::ChangeDetector() {
  env = Environment::getInstance();

  width = env->getMatrixWidth();
  height = env->getMatrixHeight();
  count = width * height;

  searchRadius = 10;

  lastMatrix = new bool[count];
  activeMatrix = new bool[count];
  velocityArray = new float[2*count];

  for (unsigned int i = 0; i < count; ++i) {
    lastMatrix[i] = false;
    activeMatrix[i] = false;
    velocityArray[i] = 0.0;
    velocityArray[i * 2] = 0.0;
  }

  memcpy(lastMatrix, env->getMatrix(), count * sizeof(bool));
}

/**
 * Deletes the changedetector.
 */
ChangeDetector::~ChangeDetector() {
  delete[] lastMatrix;
  delete[] activeMatrix;
  delete[] velocityArray;
}

/**
 * Updates the velocity and change detection. Call this method
 * periodically (i.e. in Effect::animate()) for best results, as
 * the detection of changes relies on the differences between
 * the current matrix and the matrix from  the previous call to 
 * update().
 */
void ChangeDetector::update() {
  // For performance reasons, all values are treated as integers so
  // that the comparisons can be executed for several bits at once.
  int intSize = (count * sizeof(bool)) / sizeof(int);
  int *intMatrix = (int*) env->getMatrix();
  int *actMatrix = (int*) activeMatrix;
  int *lstMatrix = (int*) lastMatrix;

  for (int i = 0; i < intSize; ++i) {
    actMatrix[i] = intMatrix[i] & (~lstMatrix[i]);
  }

  memset(velocityArray, 0, 2 * count * sizeof(float));

  float ssqr = searchRadius * searchRadius;

  for (unsigned int i = 0; i < height; ++i) {
    for (unsigned int t = 0; t < width; ++t) {
      if (activeMatrix[i* width + t]) {
        int vidx = (i * width + t) * 2;

        int dir[2] = { searchRadius+1 , searchRadius+1 };
        float dist = ssqr;

        unsigned int  xstart = (searchRadius >= t) ? 0 : (t - searchRadius);
        unsigned int  ystart = (searchRadius >= i) ? 0 : (i - searchRadius);

        unsigned int  xend = (t + searchRadius > width) ? width : (t + searchRadius);
        unsigned int  yend = (i + searchRadius > height) ? height : (i + searchRadius);

        for (unsigned int si = ystart; si < yend; ++si) {
          for (unsigned int st = xstart; st < xend; ++st) {
            if (lastMatrix[si * width + st]) {
              int currHit[] = {t-st, i - si};
              float currDist = hypot(currHit[0], currHit[1]);
              if (currDist < dist) {
                dist = currDist;
                dir[0] = currHit[0];
                dir[1] = currHit[1];
              }
            }
          }
        }

        if (dist < ssqr) {
          velocityArray[vidx]   = dir[0]/30.0;
          velocityArray[vidx+1] = dir[1]/30.0;
        }
      }
    }
  }

  memcpy(lastMatrix, env->getMatrix(), count * sizeof(bool));
}

/**
 * Check for activity at a certain position.
 * 
 * \param x X coordinate of position.
 * \param y Y coordinate of position.
 * 
 * \return True if there is activity at the position, else false.
 */
bool ChangeDetector::isActive(unsigned int x, unsigned int y) {
  return activeMatrix[x + y * width];
}

/**
 * Gets the force and direction of the changes at a certain position.
 * The vector between the position in the arguments and the returned
 * position indicates the direction and the force of the activity. The
 * returned array contains two elements, the first value corresponds 
 * to the X coordinate and the second value to the Y coordinate.
 * 
 * \param x X coordinate of position.
 * \param y Y coordinate of position.
 * 
 * \return Array with two elements indicating force and direction.
 */
const float* ChangeDetector::getVelocity(unsigned int x, unsigned int y) {
  return const_cast<const float*>(&velocityArray[(x + y * width) * 2]);
}
