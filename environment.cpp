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
}

Environment::~Environment() {
}

Environment* Environment::getInstance() {
  if (!instance) {
    instance = new Environment();
  }

  return instance;
}
