//
// C++ Implementation: catchsparkseffect
//
// Description:
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <GL/glew.h>
#include "catchsparkseffect.h"
#include "flower.h"
#include "RGBAbmpLoader.h"

CatchSparksEffect::CatchSparksEffect(EffectSettings *conf)
    : Effect()
{
  flowerCount = 10;
  blossomCount = 5;
  sparkCount = 40;
  xd = 4.0 / env->getMatrixWidth();
  yd = 2.0 / env->getMatrixHeight();
  
  
  Flower::setSparkCount(sparkCount);
  Flower::setBlossomCount(blossomCount);
  Flower::setAreaDetector(&detector);
  Flower::setxyd(xd, yd);
  flowers = new Flower[flowerCount];
  
  int verticesTotal = flowerCount * (blossomCount + sparkCount);
  vertices = new GLfloat[2 * verticesTotal];
  colors = new GLfloat[4 * verticesTotal];
  
  for ( int i = 0; i < verticesTotal; ++i) {
    vertices[2*i] = 0.0;
    vertices[2*i+1] = 0.0;
    colors[4*i] = 1.0;
    colors[4*i+1] = 1.0;
    colors[4*i+2] = 1.0;
    colors[4*i+3] = 1.0;
  }
  
  unsigned int sparkoffset = blossomCount * 2 * flowerCount; // sparks are stored behind blossoms
  for (unsigned int i = 0; i < flowerCount; ++i) {
    flowers[i].setBlossomPointers(vertices + i * 2 * blossomCount, colors + i * 4 * blossomCount);
    flowers[i].setSparksPointers(vertices + sparkoffset + i * 2 * sparkCount, colors + 2*sparkoffset + i * 4 * sparkCount);
    flowers[i].reset();
  }
  
  RGBAbmp *pic = loadBmp("data/blossom.bmp");
  
  glGenTextures(1, &blossomTex);
  glBindTexture(GL_TEXTURE_2D, blossomTex);
  int textureType = GL_RGB;
  if (pic->bpp == 32) 
  textureType = GL_RGBA ;
  gluBuild2DMipmaps(GL_TEXTURE_2D, pic->bpp/8, pic->width, 
                                    pic->height, textureType, GL_UNSIGNED_BYTE, pic->data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);	
  delete pic->data;
  delete pic;
  
  pic = loadBmp("data/whiteflare.bmp");
  
  glGenTextures(1, &sparkTex);
  glBindTexture(GL_TEXTURE_2D, sparkTex);
  if (pic->bpp == 32) 
  textureType = GL_RGBA ;
  gluBuild2DMipmaps(GL_TEXTURE_2D, pic->bpp/8, pic->width, 
                                    pic->height, textureType, GL_UNSIGNED_BYTE, pic->data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);	
  delete pic->data;
  delete pic;

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  
  glVertexPointer(2, GL_FLOAT,0, vertices);
  glColorPointer(4, GL_FLOAT,0, colors);


    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_DST_ALPHA );
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

    glPointSize( 15.0 );

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
}


CatchSparksEffect::~CatchSparksEffect()
{
    glDisable( 0x8861 );
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    delete[] flowers;
    delete[] vertices;
    delete[] colors;
}


void CatchSparksEffect::animate ( int t )
{
  detector.update();
  const AreaObjects &objects = detector.getObjects();

/*  for(unsigned int i = 0; i < objects.size(); ++i) {
    unsigned int width = objects[i].width; 

  }*/
  for (unsigned i = 0; i < flowerCount; ++i) {
    if ( flowers[i].getFetched() == 0) {
      for(unsigned int obj = 0; (obj < objects.size()) && (flowers[i].getFetched() == 0); ++obj) {
        float  dif[] = {flowers[i].position[0] - (objects[obj].x * xd - 2.0), flowers[i].position[1] - (objects[obj].y * yd -1.0 )};
        if (hypot(dif[0], dif[1]) < (objects[obj].width*xd))
          flowers[i].setFetched(objects[obj].id);
      }
    }
    flowers[i].animate();
  }
}

void CatchSparksEffect::draw()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glPointSize(10.0 );
  glBindTexture(GL_TEXTURE_2D, sparkTex);
  glDrawArrays(GL_POINTS, blossomCount * flowerCount, flowerCount * sparkCount);
  
  glColor4f(1.0, 1.0, 1.0, 0.7);
  glPointSize( 12.0 );
  glBindTexture(GL_TEXTURE_2D, blossomTex);
  glDrawArrays(GL_POINTS, 0, flowerCount*blossomCount);

}

