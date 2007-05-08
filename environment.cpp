//
// C++ Implementation: environment
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "environment.h"

Environment* Environment::instance = 0;

Environment::Environment() {
  screenWidth = 1067;
  screenHeight = 400;
  fps = 25;
  matrixSize[0] = 128;
  matrixSize[1] = 128;
  matrix = (bool*)malloc(sizeof(bool)*matrixSize[0]*matrixSize[1]);
}

Environment::~Environment() {
  free(matrix);
}

Environment* Environment::getInstance() {
  if (!instance) {
    instance = new Environment();
  }

  return instance;
}
