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

class OptionSender;

/**
	@author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class Environment{
public:
    ~Environment();

    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }
    int getFps() const { return fps; }
    int getMatrixWidth() const { return matrixWidth; }
    int getMatrixHeight() const { return matrixHeight; }
    bool loadConfig(std::string filename);
    bool *getMatrix() const { return matrix; }
    static Environment* getInstance();
    std::size_t getEffectCount();
    EffectSettings * getConfigFor(std::size_t n);
    bool getMousesimulation() const { return mousesimulation; }
    int getMouseRadius() const { return mouseRadius; }
    void updateMatrixDimensions(EffectSettings *conf);
    bool isFullscreen();

  private:
    Environment();

    int screenWidth;
    int screenHeight;
    int fps;
    bool mousesimulation;  ///< Defines whether input is take from the mouse ot the table.

    // The one and only instance of the environemnt!
    static Environment *instance;
    int matrixWidth;
    int matrixHeight;
    bool *matrix;
    std::vector<EffectSettings> configurations;
    EffectSettings globalconfig;
    int mouseRadius;

    #ifndef NOVRF
    bool fullscreen;
      OptionSender *sender;
    #endif

};

#endif
