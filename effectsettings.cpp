//
// C++ Implementation: effectsettings
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "effectsettings.h"

EffectSettings::EffectSettings(std::string name) : name(name) {}


EffectSettings::~EffectSettings()
{
}

std::ostream& operator << (std::ostream& os, const EffectSettings& s) {
  return os << s.getName() << std::endl;
}


void EffectSettings::addProperty(std::string key, std::string value) {
  properties[key] =  value;
}


/**
 * @return the value for 'key' and try to converted it to a float
 */
float EffectSettings::getFloat(std::string key) {
  return atof(properties[key].c_str());
}


/**
 * @return the value for 'key' and try to converted it to an integer
 */
int EffectSettings::getInteger(std::string key)
{
  return atoi(properties[key].c_str());
}

/**
 * @return the value for 'key'
 */
const std::string& EffectSettings::getString(std::string key) {
  return properties[key];
}
