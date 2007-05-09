//
// C++ Implementation: simplesparkle
//
// Description:
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "simplesparkle.h"
#include <iostream>
#include "RGBAbmpLoader.h"
#include <math.h>
#include "effectsettings.h"


SimpleSparkle::SimpleSparkle()
: Effect() {
    variance = 0.3;
    maxSpeed = 0.0;
    aging = 0.96;
    init();
}

SimpleSparkle::SimpleSparkle(EffectSettings *conf)
: Effect() {
    variance = conf->getFloat("variance");
    maxSpeed = conf->getFloat("maxspeed");
    aging = conf->getFloat("aging");
    init();
}


void SimpleSparkle::init() {
    int mw = env->getMatrixWidth(), mh = env->getMatrixHeight();
    xd = 4.0 / mw;
    yd = 2.0 / mh;
    count = mw * mh;

 RGBAbmp *pic = loadBmp("rgba.bmp");
  glGenTextures(1, &particleTex);
  glBindTexture(GL_TEXTURE_2D, particleTex);
  int textureType = GL_RGB;
  if (pic->bpp == 32) 
  textureType = GL_RGBA ;
  gluBuild2DMipmaps(GL_TEXTURE_2D, pic->bpp/8, pic->width, 
                                    pic->height, textureType, GL_UNSIGNED_BYTE, pic->data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);	
  delete pic->data;
  delete pic;

  
  vertexArray = (GLfloat*)malloc(sizeof(GLfloat) * 2 * mw * mh) ;
  vel = (GLfloat*)malloc(sizeof(GLfloat) * 2 * mw * mh) ;
  colorArray  = (GLfloat*)malloc(sizeof(GLfloat) * 4 * count) ;
  for (int i = 0; i < mh; ++i )
      for (int t = 0; t < mw; ++t ) {
        int vidx = (i*mw+t)*2;
        vertexArray[vidx] = t*xd - 2.0 +  ((((float)rand()/RAND_MAX)*variance)-variance*0.5);
        vertexArray[vidx+1] =  i * yd - 1.0 + ((((float)rand()/RAND_MAX)*variance)-variance*0.5);
        vel[vidx] = 0.0;
        vel[vidx+1] = 0.0;
        int cidx = (i*mw+t)*4;
        colorArray[cidx] = 1.0;
        colorArray[cidx+1] = 1.0;
        colorArray[cidx+2] = 1.0;
        colorArray[cidx+3] = 0.9;
      }
        
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  
  glVertexPointer(2, GL_FLOAT,0, vertexArray);
  glColorPointer(4, GL_FLOAT,0, colorArray);
}

SimpleSparkle::~SimpleSparkle() {
  free(vertexArray);
  free(colorArray);
  free(vel);
}


void SimpleSparkle::animate(int t) {
  int mw = env->getMatrixWidth(), mh = env->getMatrixHeight();
  bool* matrix = env->getMatrix();
  int cidx, vid;
  float angle, speed;
  for (int i = 0; i < mh; ++i )
      for (int t = 0; t < mw; ++t ) {
        cidx = (i*mw+t)*4;
        vid = (i*mh+t)*2; 
        if (( matrix[i*mw+t] ) && ( colorArray[cidx+3] < 0.9 )) {
          colorArray[cidx+3] = 0.9;
          vertexArray[vid] = t*xd - 2.0;
          vertexArray[vid+1] =  i * yd - 1.0;
          angle = ((float)rand()/RAND_MAX) * M_PI * 2.0;
          speed = ((float)rand()/RAND_MAX) * maxSpeed;
          vel[vid] = sin(angle)*speed;
          vel[vid+1] = cos(angle)*speed;
        } 
        if (colorArray[cidx+3] > 0.0 ) {
          colorArray[cidx+3] *= 0.95;
           vertexArray[vid]   += vel[vid];
           vertexArray[vid+1] += vel[vid+1];
        }
/*        if ( matrix[i*mw+t] )
          colorArray[cidx+3] = 1.0;
        else
          colorArray[cidx+3] = 0.0;*/
      }
}

void SimpleSparkle::draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
        // This is how will our point sprite's size will be modified by 
    // distance from the viewer
    float quadratic[] =  { 1.0f, 0.0f, 0.01f };
    glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );

    // Query for the max point size supported by the hardware
    float maxSize = 0.0f;
    glGetFloatv( GL_POINT_SIZE_MAX_ARB, &maxSize );

    // Clamp size to 100.0f or the sprites could get a little too big on some  
    // of the newer graphic cards. My ATI card at home supports a max point 
    // size of 1024.0f!
    if( maxSize > 100.0f )
        maxSize = 100.0f;

    glPointSize( 35.0 );

    // The alpha of a point is calculated to allow the fading of points 
    // instead of shrinking them past a defined threshold size. The threshold 
    // is defined by GL_POINT_FADE_THRESHOLD_SIZE_ARB and is not clamped to 
    // the minimum and maximum point sizes.
    //glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );

    glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 1.0f );
    glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, maxSize );

    // Specify point sprite texture coordinate replacement mode for each 
    // texture unit
    glTexEnvf( 0x8861, 0x8862, GL_TRUE );

    glEnable( 0x8861 );


    glDrawArrays(GL_POINTS, 0, count);
//     glBegin(GL_POINTS);
//       glVertex2f(vertexArray[1], 0.0);
//     glEnd();
    glDisable( 0x8861 );
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

}

