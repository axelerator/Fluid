//
// C++ Implementation: areadetector
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "areadetector.h"

#include <string.h>

/**
 * Creates an areadetector.
 *
 * \todo Make some settings configurable.
 */
AreaDetector::AreaDetector() : threshold(5.0), lastId(0) {
  env = Environment::getInstance();

  width = env->getMatrixWidth();
  height = env->getMatrixHeight();
  count = width * height;

  workMatrix = new bool[count];
}

/**
 * Deletes the detector.
 */
AreaDetector::~AreaDetector() {
  delete[] workMatrix;
}

/**
 * Update the detected objects. That is, discovering new objects or
 * discarding old objects or detecting moved objects. This should 
 * be called periodically (i.e. in Effect::animate()) or every time
 * before using the detector.
 */
void AreaDetector::update() {
  AreaObjects newObjects;
  AreaObjects oldObjects = objects;

  objects.clear();
  memset(workMatrix, false, count * sizeof(bool));

  // Discover all objects.
  for (unsigned int x = 0; x < width; ++x) {
    for (unsigned int y = 0; y < height; ++y) {
      unsigned int currentIndex = x + y * width;

      if (! workMatrix[currentIndex]) {
        // This cell was not checked yet
        if (env->getMatrix()[currentIndex]) {
          // The current cell is being "touched", we found an object
          newObjects.push_back(identifyObject(x, y));
        }
      }
    }
  }

  // Check for objects that have already existed before (i.e. moving objects).
  for (unsigned int i = 0; i < newObjects.size(); ++i) {
    unsigned int j = 0;
    bool found = false;
    AreaObjects::iterator iter = oldObjects.begin();
    while (!found && j < oldObjects.size()) {
      unsigned int maxX = newObjects[i].x > oldObjects[j].x ? newObjects[i].x : oldObjects[j].x;
      unsigned int minX = newObjects[i].x < oldObjects[j].x ? newObjects[i].x : oldObjects[j].x;
      unsigned int maxY = newObjects[i].y > oldObjects[j].y ? newObjects[i].y : oldObjects[j].y;
      unsigned int minY = newObjects[i].y < oldObjects[j].y ? newObjects[i].y : oldObjects[j].y;

      // Check if the object is within the range specified by threshold.
      float dist = sqrt( ((maxX - minX)^2) + ((maxY - minY)^2) );
      if(dist < threshold) {
        found = true;
      }
      else {
        ++j;
        ++iter;
      }
    }

    if (found) {
      // The object exists already within the range specified by threshold.
      newObjects[i].id = oldObjects[j].id;
      oldObjects.erase(iter);
    }
    else {
      // The object seems to be new, therefore a new identifier is generated.
      newObjects[i].id = ++lastId;
    }

    objects.push_back(newObjects[i]);
  }
}

/**
 * Discover the properties (width, height, exact position) of the object 
 * detected at a certain cell of the matrix.
 *
 * \param x X coordinate of the object (covered cell).
 * \param y Y coordinate of the object (covered cell).
 *
 * \return The properties of the identified object.
 */
AreaObject AreaDetector::identifyObject(unsigned int x, unsigned int y) {
  AreaObject result;

  Border initialBorder;
  initialBorder.maxX = 0;
  initialBorder.minX = width - 1;
  initialBorder.maxY = 0;
  initialBorder.minY = height - 1;

  // Recursively discover all connected cells that belong to the object.
  Border objectBorder = checkNeighbours(x, y, initialBorder);

  result.width = objectBorder.maxX - objectBorder.minX;
  result.height = objectBorder.maxY - objectBorder.minY;

  result.x = objectBorder.minX + result.width / 2;
  result.y = objectBorder.minY + result.height / 2;

  return result;
}

/**
 * Recursively check all neighbouring cells if they belong to the same 
 * object. While doing this, the dimensions (in terms of width and height)
 * of the object are calculated.
 *
 * \param x X coordinate of a covered cell belonging to the object.
 * \param y Y coordinate of a covered cell belonging to the object.
 * \param currentBorder The current dimensions of the bounding box of the object so far.
 *
 * \return The new dimensions of the bounding box if neighbouring covered cells are found.
 */
Border AreaDetector::checkNeighbours(unsigned int x, unsigned int y, Border currentBorder) {
  unsigned int currentIndex = x + y * width;
  unsigned int leftIndex = x - 1 + y * width;
  unsigned int rightIndex = x + 1 + y * width;
  unsigned int topIndex = x + (y + 1) * width;
  unsigned int bottomIndex = x + (y - 1) * width;

  workMatrix[currentIndex] = true;

  if (x > currentBorder.maxX) currentBorder.maxX = x;
  if (x < currentBorder.minX) currentBorder.minX = x;
  if (y > currentBorder.maxY) currentBorder.maxY = y;
  if (y < currentBorder.minY) currentBorder.minY = y;

  Border result = currentBorder;

  if ((x > 0) && env->getMatrix()[leftIndex] && !workMatrix[leftIndex]) {
    Border leftBorder = checkNeighbours(x - 1, y, currentBorder);
    if (leftBorder.maxX > result.maxX) result.maxX = leftBorder.maxX;
    if (leftBorder.minX < result.minX) result.minX = leftBorder.minX;
    if (leftBorder.maxY > result.maxY) result.maxY = leftBorder.maxY;
    if (leftBorder.minY < result.minY) result.minY = leftBorder.minY;
  }

  if ((x < width - 1) && env->getMatrix()[rightIndex] && !workMatrix[rightIndex]) {
    Border rightBorder = checkNeighbours(x + 1, y, currentBorder);
    if (rightBorder.maxX > result.maxX) result.maxX = rightBorder.maxX;
    if (rightBorder.minX < result.minX) result.minX = rightBorder.minX;
    if (rightBorder.maxY > result.maxY) result.maxY = rightBorder.maxY;
    if (rightBorder.minY < result.minY) result.minY = rightBorder.minY;
  }

  if ((y > 0) && env->getMatrix()[bottomIndex] && !workMatrix[bottomIndex]) {
    Border bottomBorder = checkNeighbours(x, y - 1, currentBorder);
    if (bottomBorder.maxX > result.maxX) result.maxX = bottomBorder.maxX;
    if (bottomBorder.minX < result.minX) result.minX = bottomBorder.minX;
    if (bottomBorder.maxY > result.maxY) result.maxY = bottomBorder.maxY;
    if (bottomBorder.minY < result.minY) result.minY = bottomBorder.minY;
  }

  if ((y < height - 1) && env->getMatrix()[topIndex] && !workMatrix[topIndex]) {
    Border topBorder = checkNeighbours(x, y + 1, currentBorder);
    if (topBorder.maxX > result.maxX) result.maxX = topBorder.maxX;
    if (topBorder.minX < result.minX) result.minX = topBorder.minX;
    if (topBorder.maxY > result.maxY) result.maxY = topBorder.maxY;
    if (topBorder.minY < result.minY) result.minY = topBorder.minY;
  }

  return result;
}
