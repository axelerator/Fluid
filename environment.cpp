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
#include <vector>
#include <fstream>
#include <iostream>
#include "vrf/include/OptionSender.h"

Environment* Environment::instance = 0;

Environment::Environment() 
: mousesimulation(true), globalconfig(std::string("environment")) {

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

bool Environment::loadConfig(std::string filename) { 
    std::vector<std::string> file;
    std::string line;
    file.clear();
    std::ifstream infile (filename.c_str(), std::ios_base::in );
    if ( !infile.is_open()) {
      std::cout << "config file \"" << filename << "\" not found!" << std::endl;
      return false;
    }
    while (getline(infile, line, '\n'))
          file.push_back (line);
    
    std::vector<std::string>::const_iterator cii;
    EffectSettings *currentSetting = 0;
    for( cii = file.begin(); cii != file.end(); cii++) {
      if ((*cii).length() > 0) {
        std::string currLine = *cii; 
        while ((currLine.at(0) == ' ') || (currLine.at(0) == '\t'))
          currLine = currLine.substr(1,currLine.size()-1);
        if (currLine.at(0) == 35) {
          ; // skip comment line
        } else if (currLine.at(0) == 91) {
          std::size_t closing = currLine.find_first_of(']', 0);
          std::string effectName = currLine.substr(1, closing-1);
          if ( ! effectName.compare("environment") ) {
            currentSetting = &globalconfig;
          } else {
            configurations.push_back(EffectSettings(effectName));
            currentSetting = &(configurations.back());
          
          }
        } else {
          if (currentSetting == 0 )
            std::cout << "Property not assigned to any block. Ignoring(" << currLine << ")";
          else {
            std::size_t propNameEnd = currLine.find_first_of('=');
            if ( propNameEnd == std::string::npos )
              std::cerr << "No value for property \"" << currLine << "\". Seperate with '=' !" << std::endl;
            else {
              if (currLine.find_first_of('\t') < propNameEnd)
                propNameEnd = currLine.find_first_of('\t');
              if (currLine.find_first_of(' ') < propNameEnd)
                propNameEnd = currLine.find_first_of(' ');
              std::string propName = currLine.substr(0, propNameEnd);
              std::string value = currLine.substr(currLine.find_first_of('=')+1, currLine.size()-1);
              while ((value.at(0) == ' ') || (value.at(0) == '\t'))
                value = value.substr(1,value.size()-1);
              //std::cout << "prpoerty:" << propName << ":" << value << ":" << std::endl;
              currentSetting->addProperty( propName, value );
            }
          }
        }
      }
    }
  screenWidth = globalconfig.getInteger("screenwidth");
  screenHeight = globalconfig.getInteger("screenheight");
  fps = globalconfig.getInteger("fps");
  matrixSize[0] = globalconfig.getInteger("matrixwidth");
  matrixSize[1] = globalconfig.getInteger("matrixheight");
  matrix = (bool*)malloc(sizeof(bool)*matrixSize[0]*matrixSize[1]);
  mousesimulation = globalconfig.getString("mousesimulation").compare("off") != 0;
  if (!mousesimulation) {
    OptionSender* sender = new OptionSender();
    sender->SetNewOption(BOOLMATRIX, matrixSize[0], matrixSize[1], matrix);
  }
  return true; 
}


/**
 * @return the amount of configurations, which determines the amount of different Effects
 */
std::size_t Environment::getEffectCount() {
  return configurations.size();
}


/**
 * @return the settings for th n-th effect
 */
EffectSettings * Environment::getConfigFor(std::size_t n) {
  return &(configurations.at(n));
}
