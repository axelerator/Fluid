//
// C++ Implementation: changedetector
//
// Description:
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "changedetector.h"
#include <math.h>


ChangeDetector::ChangeDetector()
    : Effect()
{
  int mw = env->getMatrixWidth(), mh = env->getMatrixHeight();
  float xd = 4.0 / mw;
  float yd = 2.0 / mh;
  count = mw * mh;
  searchRadius = 10;
  lastMatrix = new bool[count];
  activeMatrix = new bool[count];
  vertexArray = new GLfloat[2 * count];
  colorArray  = new GLfloat[3 * count];
  velocityArray = new GLfloat[2*count];

  for ( int i = 0; i < mh; ++i )
    for ( int t = 0; t < mw; ++t )
    {
      int vidx = ( i*mw+t ) *2;
      vertexArray[vidx] = t*xd - 2.0 ;
      vertexArray[vidx+1] =  i * yd - 1.0 ;
      int cidx = ( i*mw+t ) * 3;
      colorArray[cidx] = 0.3;
      colorArray[cidx+1] = 0.3;
      colorArray[cidx+2] = 0.3;
    }

  glEnableClientState ( GL_VERTEX_ARRAY );
  glEnableClientState ( GL_COLOR_ARRAY );

  glVertexPointer ( 2, GL_FLOAT,0, vertexArray );
  glColorPointer ( 3, GL_FLOAT,0, colorArray );
  glPointSize ( 1.0 );
  memcpy(lastMatrix, env->getMatrix(), count * sizeof(bool));
  memset(velocityArray, 0, 2*count*sizeof(GLfloat));

}


ChangeDetector::~ChangeDetector()
{
  delete[] lastMatrix;
  delete[] vertexArray;
  delete[] velocityArray;
  delete[] colorArray;
  delete[] activeMatrix;
}


void ChangeDetector::animate ( int t ) {
  int intSize = (count * sizeof(bool)) / sizeof(int);
  int *intMatrix = (int*)env->getMatrix();
  int *actMatrix = (int*)activeMatrix;
  int *lstMatrix = (int*)lastMatrix;
  for (int i = 0; i < intSize; ++i) {
    actMatrix[i] = intMatrix[i] & (~lstMatrix[i]);
  }
  for (std::size_t i = 0; i < count; ++i )
  {
    int cidx = ( i ) * 3;
     if ( activeMatrix[i] ) {
      colorArray[cidx] = 1.0;
      colorArray[cidx+1] = 1.0;
      colorArray[cidx+2] = 1.0;
    } else { 
      colorArray[cidx] = 0.3;
      colorArray[cidx+1] = 0.3;
      colorArray[cidx+2] = 0.3;
    }
  }
  memset(velocityArray, 0, 2*count*sizeof(GLfloat));
  std::size_t mw = env->getMatrixWidth(), mh = env->getMatrixHeight();
  float ssqr = searchRadius*searchRadius;
  for ( std::size_t i = 0; i < mh; ++i )
    for ( std::size_t t = 0; t < mw; ++t )
    {
      if ( activeMatrix[i*mw+t] ) {
        int vidx = ( i*mw+t ) *2;
        
        int dir[2] = { searchRadius+1, searchRadius+1 };
        float dist = ssqr;
        
        std::size_t  xstart = (searchRadius >= t) ? 0 : (t - searchRadius);
        std::size_t  ystart = (searchRadius >= i) ? 0 : (i - searchRadius);
        
        std::size_t  xend = (t + searchRadius > mw) ? mw : (t + searchRadius);
        std::size_t  yend = (i + searchRadius > mh) ? mh : (i + searchRadius);
        
        for (std::size_t si = ystart; si < yend; ++si)
          for (std::size_t st = xstart; st < xend; ++st) {
            if ( lastMatrix[si*mw+st] ) {
              int currHit[] = {t-st, i - si};
              float currDist = hypot(currHit[0], currHit[1]);
              if (currDist < dist) {
                dist = currDist;
                dir[0] = currHit[0];
                dir[1] = currHit[1];
              }
            }
        }
        if ( dist < ssqr) {
          velocityArray[vidx]   = dir[0]/30.0;
          velocityArray[vidx+1] = dir[1]/30.0;
        }
      }
    }
  
  memcpy(lastMatrix, env->getMatrix(), count * sizeof(bool));
}

void ChangeDetector::draw()
{
  glClear ( GL_COLOR_BUFFER_BIT );
  glMatrixMode ( GL_MODELVIEW );
  glLoadIdentity ();
  glDrawArrays ( GL_POINTS, 0, count );
  
  std::size_t twoI;
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  for (std::size_t i = 0; i < count; ++i )
    if (activeMatrix[i]) {
      twoI = 2 * i;
      glVertex2fv(vertexArray + twoI );
      glVertex2f(vertexArray[twoI] + velocityArray[twoI], vertexArray[twoI+1] + velocityArray[twoI+1]);
    }
  glEnd();
}

