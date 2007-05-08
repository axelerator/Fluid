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
    int getMatrixWidth() const { return matrixSize[0]; }
    int getMatrixHeight() const { return matrixSize[1]; }
    bool loadConfig(std::string filename) { return true; }
    bool *getMatrix() const { return matrix; }
    static Environment* getInstance();
  private:
    Environment();

    int screenWidth;
    int screenHeight;
    int fps;

    // The one and only instance of the environemnt!
    static Environment *instance;
    int matrixSize[2];
    bool *matrix;
};

#endif
