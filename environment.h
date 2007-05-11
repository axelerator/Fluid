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
#include <vector>
#include "effectsettings.h"
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
    bool loadConfig(std::string filename);
    bool *getMatrix() const { return matrix; }
    static Environment* getInstance();
    std::size_t getEffectCount();
    EffectSettings * getConfigFor(std::size_t n);
    bool getMousesimulation() const { return mousesimulation; }
	
  private:
    Environment();

    int screenWidth;
    int screenHeight;
    int fps;
    /**
     * defines wther input is take from the mouse ot the table
     */
    bool mousesimulation;

    // The one and only instance of the environemnt!
    static Environment *instance;
    int matrixSize[2];
    bool *matrix;
    std::vector<EffectSettings> configurations;
    EffectSettings globalconfig;
};

#endif
