//
// C++ Interface: effectsettings
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef EFFECTSETTINGS_H
#define EFFECTSETTINGS_H
#include <string>
#include <map>
#include <iostream>

/**
Encapsulates the configuration for an effect and provides conveniece access methods.

	@author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class EffectSettings {
public:
    EffectSettings(std::string name);
    ~EffectSettings();

    const std::string getName() const { return name;}
    void addProperty(std::string key, std::string value);

    float getFloat(std::string key);
    int getInteger(std::string key);
    const std::string& getString(std::string key);

    float getFloatOrDefault(std::string key, float defaultValue);
    int getIntegerOrDefault(std::string key, int defaultValue);
    std::string getStringOrDefault(std::string key, std::string defaultValue);

private:
    std::string name;
    std::map <std::string, std::string> properties ;
    static std::string undefined; 
};

std::ostream& operator<<(std::ostream& os, const EffectSettings& s);
#endif
