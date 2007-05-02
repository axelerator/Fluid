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

class Environment;
class Effect;

/**
	@author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class EffectManager {
  public:
    ~EffectManager();

    Environment* getEnvironment();

    void draw();
    void animate(int t);

    static EffectManager* getInstance();

  private:
    EffectManager();

    Environment *env;
    Effect *currentEffect;

    // The one and only effect manager instance!
    static EffectManager *instance;
};

#endif
