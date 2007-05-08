//
// C++ Implementation: effectmanager
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "effectmanager.h"
#include "environment.h"
#include "effect.h"
#include "simplesparkle.h"
#include "matrixviseffect.h"

EffectManager* EffectManager::instance = 0;

EffectManager::EffectManager() {
  env = Environment::getInstance();


}

EffectManager::~EffectManager() {
}

void EffectManager::init() {
  currentEffect = new SimpleSparkle();
//  currentEffect = new MatrixVisEffect();
  currentEffect->init();
}

EffectManager* EffectManager::getInstance() {
  if (!instance) {
    instance = new EffectManager();
  }
  return instance;
}

Environment* EffectManager::getEnvironment() {
  return env;
}

void EffectManager::draw() {
  currentEffect->draw();
}

void EffectManager::animate(int t) {
  currentEffect->animate(t);
}
