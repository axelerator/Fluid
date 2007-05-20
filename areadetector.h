//
// C++ Interface: areadetector
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AREADETECTOR_H
#define AREADETECTOR_H

#include <vector>
#include <math.h>

#include "environment.h"

/**
 * Representing an identified object with it's position, size and id.
 */
struct AreaObject {
  unsigned int x;        ///< X coordinate of the object position.
  unsigned int y;        ///< Y coordinate of the object position.

  unsigned int width;    ///< Approximated width of the detected object.
  unsigned int height;   ///< Approximated height of the detected object.

  unsigned int id;       ///< Unique identifier of the detected object.
};

typedef std::vector<AreaObject> AreaObjects;

/**
* Used for describing the bounding box of a detected object (only used internally).
*/
struct Border {
  unsigned int minX;     ///< Minimum X of the bounding box.
  unsigned int minY;     ///< Minimum Y of the bounding box.

  unsigned int maxX;     ///< Maximum X of the bounding box.
  unsigned int maxY;     ///< Maximum Y of the bounding box.
};

/**
 * Simple detection of covered areas in the matrix. These areas are identified as
 * objects and the detection attempts to assign those objects a unique identifier
 * that will not change if the object is moving.
 */
class AreaDetector {
  public:
    AreaDetector();
    ~AreaDetector();

    void update();
    const AreaObjects& getObjects() { return objects; }

  private:
    AreaObject identifyObject(unsigned int x, unsigned int y);
    Border checkNeighbours(unsigned int x, unsigned int y, Border currentBorder);

    unsigned int count;   ///< Number of elements in the matrix (just saved here for convenience & performance).
    unsigned int width;   ///< Width of the matrix (just saved here for convenience & performance).
    unsigned int height;  ///< Height the matrix (just saved here for convenience & performance).

    Environment *env;     ///< The environment (needed here because the AreaDetector is not inherited from Effect).

    bool *workMatrix;     ///< This matrix is used for temporary data during detection.

    AreaObjects objects;  ///< Identified objects.
    float threshold;      ///< Threshold for identifying moving objects as unique.
    unsigned int lastId;  ///< Identifier of the most recently detected object.
};

#endif
