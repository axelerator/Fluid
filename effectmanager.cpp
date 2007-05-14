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
#include "simpleeffect.h"
#include "matrixviseffect.h"
#include "changedetector.h"
#include "fluideffect.h"
EffectManager* EffectManager::instance = 0;

EffectManager::EffectManager() :  currentEffect(0), currentEffectId(0){
    env = Environment::getInstance();
}

EffectManager::~EffectManager() {}

void EffectManager::init() {
    createEffect(currentEffectId);
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

void EffectManager::nextEffect() {
  if ( currentEffectId < (env->getEffectCount()-1) )
    createEffect(++currentEffectId);
}

void EffectManager::previousEffect() {
  if ( currentEffectId > 0)
    createEffect(--currentEffectId);
}

void EffectManager::createEffect(std::size_t n) {
    if  ( currentEffect != 0 )
      delete currentEffect;
    EffectSettings *nextSetting = env->getConfigFor(currentEffectId );
    std::cout << "Switching to next Effect.." << nextSetting->getName() << std::endl;
    if ( !nextSetting->getName().compare("simplesparkle") )
      currentEffect = new SimpleSparkle(nextSetting);
    else if ( !nextSetting->getName().compare("simple") )
      currentEffect = new SimpleEffect();
    else if ( !nextSetting->getName().compare("matrixvis") )
      currentEffect = new MatrixVisEffect();
    else if ( nextSetting->getName().compare("changedetector") == 0 )
     currentEffect = new ChangeDetector();
    else if ( nextSetting->getName().compare("fluid") == 0 )
     currentEffect = new FluidEffect();
    else {
      std::cerr << "Effect '" << nextSetting->getName() << "' not found -> exiting.";
      exit(1);
    }

}

