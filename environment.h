//
// C++ Interface: environment
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>

/**
	@author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class Environment{
public:
    ~Environment();

    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }
    int getFps() const { return fps; }

    bool loadConfig(std::string filename) { return true; }

    static Environment* getInstance();
  private:
    Environment();

    int screenWidth;
    int screenHeight;
    int fps;

    // The one and only instance of the environemnt!
    static Environment *instance;
};

#endif
