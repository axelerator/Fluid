//
// C++ Interface: flower
//
// Description:
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FLOWER_H
#define FLOWER_H
#include <GL/gl.h>
#include "areadetector.h"
/**
  @author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class Flower
{
  public:
    Flower();

    ~Flower();
    void animate();
    void reset();

    void setBlossomPointers(GLfloat *v, GLfloat *c);
    void setSparksPointers(GLfloat *v, GLfloat *c);

    static void setBlossomCount ( unsigned int theValue )  {  blossomCount = theValue; }
    static unsigned int getBlossomCount() { return blossomCount; } 
    static void setSparkCount ( unsigned int theValue ) {  sparkCount = theValue; }
    static unsigned int getSparkCount()  { return sparkCount; }
    static void setAreaDetector ( AreaDetector *detector) { Flower::detector = detector; }
    static void setxyd(float x, float y) { xd =x; yd = y; }

    void setFetched ( unsigned int theValue ) { if (phase == 1) {fetched = theValue; phase = 2;}}
    bool isFetched() const	{ return (phase == 2); }
    void resetSpark(unsigned int i);
  
    GLfloat position[2];

  private :
  static AreaDetector *detector;
  static unsigned int blossomCount;
  static unsigned int sparkCount;
  static float xd;
  static float yd;
  
  GLfloat *blossom;

  GLfloat *blossomColor;
  GLfloat *sparks;
  GLfloat *sparksColors;
  unsigned int *sparksAge;
  GLfloat *sparksVel;
  int age;
  unsigned char phase ;
  
  unsigned int fetched;
};

#endif
