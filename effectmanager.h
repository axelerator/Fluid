//
// C++ Interface: effectmanager
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H
#include <cstddef>
class Environment;
class Effect;

/**
	@author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class EffectManager {
  public:
    ~EffectManager();

    Environment* getEnvironment();
    void init();
    void draw();
    void animate(int t);



    static EffectManager* getInstance();
    void nextEffect();
    void previousEffect();

  private:
    EffectManager();
    void createEffect(std::size_t n);

    Environment *env;
    Effect *currentEffect;

    // The one and only effect manager instance!
    static EffectManager *instance;
    
    /**
     *  holds the index of the configuration of the
     *  the current effect.
     */
    std::size_t currentEffectId;
};

#endif
