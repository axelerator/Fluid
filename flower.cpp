//
// C++ Implementation: flower
//
// Description:
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "flower.h"
#include <stdlib.h>
#include <GL/gl.h>
#include <math.h>

unsigned int Flower::blossomCount = 0;
unsigned int Flower::sparkCount = 0;
AreaDetector *Flower::detector = 0;
float Flower::xd = 0;
float Flower::yd = 0;

Flower::Flower() {
  sparksVel = new float[2*sparkCount];
  sparksAge = new unsigned int[sparkCount];
}

void Flower::reset() {
  fetched = 0;

  age = (int)-(((float)rand()/RAND_MAX)*1000);
  position[0] = ((((float)rand()/RAND_MAX)*4.0)-2.0);
  position[1] = ((((float)rand()/RAND_MAX)*2.0)-1.0);

  GLfloat color[3];
  color[0] = 0.2 + (((float)rand()/RAND_MAX)*0.7);
  color[1] = 0.2 + (((float)rand()/RAND_MAX)*0.7);
  color[2] = 0.2 + (((float)rand()/RAND_MAX)*0.7);
  
  float angle = 0.0;
  float p = (2.0*M_PI)/(blossomCount);
  for (unsigned int i = 0; i < blossomCount; ++i) {
    angle =  (((float)rand()/RAND_MAX)*p);
    blossom[2*i]   = position[0] + sin(i*p + angle) * 0.02;
    blossom[2*i+1] = position[1] + cos(i*p + angle) * 0.02;
    blossomColor[4*i] = 0.8;
    blossomColor[4*i+1] = color[0];
    blossomColor[4*i+2] = color[1];
    blossomColor[4*i+3] = color[2];
  }
  
  for (unsigned int i = 0; i < sparkCount; ++i) 
    resetSpark(i);
  
}

void Flower::resetSpark(unsigned int i) {
    
    sparksColors[4*i] = 0.8 + (((float)rand()/RAND_MAX)*0.2);
    sparksColors[4*i+1] = 0.6 + (((float)rand()/RAND_MAX)*0.2);
    sparksColors[4*i+2] = 0.6;
    sparksColors[4*i+3] = 0.4 ;
    
    //sparksColors[4*i] = sparksColors[4*i+1] = sparksColors[4*i+2] = 0.6;
    
    float angle =  (((float)rand()/RAND_MAX)*2.0*M_PI);
    float speed =  (((float)rand()/RAND_MAX)*0.003);
    sparksAge[i] = (unsigned int)(((float)rand()/RAND_MAX)*30);
    sparksVel[2*i]   = sin(angle);
    sparksVel[2*i+1] = cos(angle);
    
    sparks[2*i] = position[0] + sparksVel[2*i] * 0.04;
    sparks[2*i+1] = position[1] + sparksVel[2*i+1] * 0.04;
    
    sparksVel[2*i]   *= speed;
    sparksVel[2*i+1] *= speed;
 
}

Flower::~Flower()
{}


void Flower::animate() {
++age;

  if (fetched > 0) {
    const AreaObject *o = 0;
    const AreaObjects &objects = detector->getObjects();
    std::size_t oi = 0;
    while ( o == 0 && oi < objects.size()) {
      if (objects[oi].id == fetched)
        o = &(objects[oi]);
      else
        ++oi;
    }
    if (o == 0)
      this->reset();
    else {
      float dif[] = {o->x * xd - 2.0, o->y * yd - 1.0};
      position[0] = blossom[0]  += (dif[0] - blossom[0]) * (0.05 );
      position[1] = blossom[1]  += (dif[1] - blossom[1]) * (0.05 );
      for ( unsigned int t = 1; t < blossomCount; ++t) {
        blossom[2 * t]      += (blossom[2 * (t-1)] - blossom[2 * t]) * 0.5;
        blossom[2 * t + 1]  += (blossom[2 * t-1] - blossom[2 * t + 1]) * 0.5;
      }
    }
  }


  for (unsigned int i = 0; i < sparkCount; ++i) 
    if ( sparksAge[i] > 80) { 
      resetSpark(i);
    } else {
      sparks[2*i] += sparksVel[2*i];
      sparks[2*i+1] += sparksVel[2*i+1];

      sparksColors[4*i+3] *= 0.999;
      ++sparksAge[i];
    }
}

void Flower::setBlossomPointers(GLfloat *v, GLfloat *c) {
  blossom = v;
  blossomColor = c;
}

void Flower::setSparksPointers(GLfloat *v, GLfloat *c) {
  sparks = v;
  sparksColors = c;
}

