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

std::string EffectSettings::undefined("undefined");

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
  if ( properties.count(key) == 0) {
    std::cerr << "Mandatory parameter " << key << " missing in configuration. Passing 0.0 instead" << std::endl;
    return 0.0;
  }
  return atof(properties[key].c_str());
}


/**
 * @return the value for 'key' and try to converted it to an integer
 */
int EffectSettings::getInteger(std::string key)
{
  if ( properties.count(key) == 0) {
    std::cerr << "Mandatory parameter " << key << " missing in configuration. Passing 0 instead" << std::endl;
    return 0;
  }
  return atoi(properties[key].c_str());
}

/**
 * @return the value for 'key'
 */
const std::string& EffectSettings::getString(std::string key) {
    if ( properties.count(key) == 0) {
    std::cerr << "Mandatory parameter " << key << " missing in configuration. Passing 0.0 instead" << std::endl;
    return undefined ;
  }
  return properties[key];
}

/**
 * Returns the value for given key or the default argument.
 * 
 * @param key The key for the value.
 * @param defaultValue The default value if the key is not present.
 * 
 * @return The value for as float.
 */
float EffectSettings::getFloatOrDefault(std::string key, float defaultValue) {
  if ( properties.count(key) == 0) {
    return defaultValue;
  }
  return atof(properties[key].c_str());
}


/**
 * Returns the value for given key or the default argument.
 * 
 * @param key The key for the value.
 * @param default The default value if the key is not present.
 * 
 * @return The value for as integer.
 */
int EffectSettings::getIntegerOrDefault(std::string key, int defaultValue) {
  if ( properties.count(key) == 0) {
    return defaultValue;
  }
  return atoi(properties[key].c_str());
}

/**
 * Returns the value for given key or the default argument.
 * 
 * @param key The key for the value.
 * @param defaultValue The default value if the key is not present.
 * 
 * @return The value for as string.
 */
std::string EffectSettings::getStringOrDefault(std::string key, std::string defaultValue) {
    if ( properties.count(key) == 0) {
    return defaultValue;
  }
  return properties[key];
}
