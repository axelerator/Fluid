//
// C++ Implementation: waveeffect
//
// Description:
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "waveeffect.h"

#include <stdlib.h>
#include <math.h>
#include "RGBAbmpLoader.h"

WaveEffect::WaveEffect(EffectSettings *conf) 
  : Effect(), width(env->getMatrixWidth()), height(env->getMatrixHeight()), frame(0) {
  count = width * height;

  input = new float[4*count];
  output = new float[4*count];

  for (unsigned int i = 0; i < 4 * count; ++i) {
    input[i] = 0.0;
    output[i] = 0.0;
  }

  float xd = 4.0 / width;
  float yd = 2.0 / height;

  vertexArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * count) ;
  normalArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * count) ;
  colorArray  = (GLfloat*)malloc(sizeof(GLfloat) * 3 * count) ;

  indices = new GLuint[(height-1) * (width*2)];

  friction  = 0.5 - ((conf->getInteger("friction") / 200.0));

  for (unsigned int i = 0; i < height; ++i ) {
    for (unsigned int t = 0; t < width; ++t ) {
      int vidx = (i*width+t)*3;
      vertexArray[vidx] = t*xd - 2.0 ;
      vertexArray[vidx+1] =  i * yd - 1.0;
      vertexArray[vidx+2] =  0.0;
      int cidx = (i*width+t)*3;
      colorArray[cidx] = 0.2;
      colorArray[cidx+1] = 0.2;
      colorArray[cidx+2] = 0.2;
    }
  }

  for (unsigned int y = 1; y < height-1; ++y ) {
    for (unsigned int x = 1; x < width-1; ++x ) {
      unsigned int idx = (y*width+x)*3;
      unsigned int top = ((y+1) * width + x) * 3;
      unsigned int left = (y * width + (x-1)) * 3;
      unsigned int right = (y * width + (x+1)) * 3;
      unsigned int bottom = ((y-1) * width + x) * 3;
      float a[3] = {vertexArray[right] - vertexArray[left], vertexArray[right+1] - vertexArray[left+1], vertexArray[right+2] - vertexArray[left+2]};
      float b[3] = {vertexArray[top] - vertexArray[bottom], vertexArray[top+1] - vertexArray[bottom+1], vertexArray[top+2] - vertexArray[bottom+2]};
      normalArray[idx]   = a[1] *  b[2] - a[2] * b[1];
      normalArray[idx+1] = a[2] *  b[0] - a[0] * b[2];
      normalArray[idx+2] = a[0] *  b[1] - a[1] * b[0];
    }
  }

  unsigned int currIdx = 0;
   for (unsigned int y = 0; y < (height-1); ++y) {
    for ( unsigned int x = 0; x < width; ++x) {
      indices[currIdx++] = y * width + x;
      indices[currIdx++] = (y+1) * width + x;
    }
  }

  RGBAbmp *pic = loadBmp(conf->getString("texture").c_str());
  glGenTextures(1, &waterTex);
  glBindTexture(GL_TEXTURE_2D, waterTex);
  int textureType = GL_RGB;
  if (pic->bpp == 32) 
  textureType = GL_RGBA ;
  gluBuild2DMipmaps(GL_TEXTURE_2D, pic->bpp/8, pic->width, 
                                    pic->height, textureType, GL_UNSIGNED_BYTE, pic->data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);	
  
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_2D);
  delete pic->data;
  delete pic;
//   gluPerspective(60.0, 1067.0/400.0, 1.0, 100.0);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  //glEnableClientState(GL_COLOR_ARRAY);
  
  glVertexPointer(3, GL_FLOAT,0, vertexArray);
  glNormalPointer(GL_FLOAT,0, normalArray);
  //glColorPointer(3, GL_FLOAT,0, colorArray);
  glPointSize(5.0);
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  
  // Light 0
  GLfloat lightDIF0[] = { 0.9, 0.9, 0.9};
  GLfloat lightSPEC0[] = { 0.7, 0.7, 0.7};
  GLfloat lightAMB0[] = { 0.1, 0.1, 0.1};
//  GLfloat lightPOS[] = { -3.0, -0.5, 1.7, 1.0};
  GLfloat lightPOS0[] = { 0.0, 0.0, 1.5, 1.0};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDIF0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAMB0);
  glLightfv(GL_LIGHT0, GL_SPECULAR,lightSPEC0);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPOS0);
  glEnable(GL_LIGHT0);
  
  
/*  // Light 0
  GLfloat lightDIF0[] = { 0.9, 0.7, 0.2};
  GLfloat lightSPEC0[] = { 0.7, 0.7, 0.7};
  GLfloat lightAMB0[] = { 0.1, 0.1, 0.1};
//  GLfloat lightPOS[] = { -3.0, -0.5, 1.7, 1.0};
  GLfloat lightPOS0[] = { -3.0, -0.5, 0.2, 1.0};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDIF0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAMB0);
  glLightfv(GL_LIGHT0, GL_SPECULAR,lightSPEC0);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPOS0);
  glEnable(GL_LIGHT0);
  
  
  // Light 1
  GLfloat lightDIF1[] = { 0.3, 0.5, 0.9};
  GLfloat lightSPEC1[] = { 0.7, 0.7, 0.7};
  GLfloat lightAMB1[] = { 0.1, 0.1, 0.1};
//  GLfloat lightPOS[] = { -3.0, -0.5, 1.7, 1.0};
  GLfloat lightPOS1[] = { 3.0, 0.5, 0.2, 1.0};
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDIF1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, lightAMB1);
  glLightfv(GL_LIGHT1, GL_SPECULAR,lightSPEC1);
  glLightfv(GL_LIGHT1, GL_POSITION, lightPOS1);
  
  */
  



  
  
  
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glColor3f(0.7, 0.7, 0.7);

  }

WaveEffect::~WaveEffect() {
  delete[] input;
  delete[] output;
  delete[] indices;

  free(vertexArray);
  free(colorArray);
  free(normalArray);
}

void WaveEffect::animate ( int t ) {
  detector.update();
  
  for (std::size_t i = 0; i < height; ++i) {
    for (std::size_t t = 0; t < width; ++t) {
      if (detector.isActive(t, i)) {
        const float* vel = detector.getVelocity(t, i);
        // directional force
        float v = hypot(vel[0], vel[1]);
        input[(t + (i) * width) * 4] = 20* (v * v);
      }
    }
  }
  
/*  if (++frame%50 == 0) {
    input[(width/2 + (height/2) * width) * 4] = 10.0;
    input[(width/2 + (height/2) * width) * 4 + 1] = 1.0;
    input[(width/2 + (height/2) * width) * 4 + 2] = 1.0;
    input[(width/2 + (height/2) * width) * 4 + 3] = 1.0;
  }*/
/*  float val = cos( frame *0.04) * 5.0; 
    input[(width/2 + (height/2) * width) * 4] = val;
    input[(width/2 + (height/2) * width) * 4 + 1] = val;
    input[(width/2 + (height/2) * width) * 4 + 2] = val;
    input[(width/2 + (height/2) * width) * 4 + 3] = val;*/
  

//   std::cout << "in 0: " << input[(width/2 + (height/2) * width) * 4] << std::endl;
//   std::cout << "in 1: " << input[(width/2 + (height/2) * width) * 4 + 1] << std::endl;
//   std::cout << "in 2: " << input[(width/2 + (height/2) * width) * 4 + 2] << std::endl;
//   std::cout << "in 3: " << input[(width/2 + (height/2) * width) * 4 + 3] << std::endl;
// 
//   std::cout << "out 0: " << output[(width/2 + (height/2) * width) * 4] << std::endl;
//   std::cout << "out 1: " << output[(width/2 + (height/2) * width) * 4 + 1] << std::endl;
//   std::cout << "out 2: " << output[(width/2 + (height/2) * width) * 4 + 2] << std::endl;
//   std::cout << "out 3: " << output[(width/2 + (height/2) * width) * 4 + 3] << std::endl;

  for (unsigned int x = 1; x < width-1; ++x) {
    for (unsigned int y = 1; y < height-1; ++y) {
      unsigned int idx = (y * width + x) * 4;

      output[idx] = -input[idx];
      output[idx] += input[idx+1];
      output[idx] += input[idx+2];
      output[idx] += input[idx+3];
      output[idx] *= friction;

      output[idx+1] = input[idx];
      output[idx+1] -= input[idx+1];
      output[idx+1] += input[idx+2];
      output[idx+1] += input[idx+3];
      output[idx+1] *= friction;

      output[idx+2] = input[idx];
      output[idx+2] += input[idx+1];
      output[idx+2] -= input[idx+2];
      output[idx+2] += input[idx+3];
      output[idx+2] *= friction;

      output[idx+3] = input[idx];
      output[idx+3] += input[idx+1];
      output[idx+3] += input[idx+2];
      output[idx+3] -= input[idx+3];
      output[idx+3] *= friction;
    }
  }

  for (unsigned int x = 1; x < width-1; ++x) {
    for (unsigned int y = 1; y < height-1; ++y) {
      unsigned int idx = (y * width + x) *4;

      unsigned int top = ((y+1) * width + x) * 4;
      unsigned int left = (y * width + (x-1)) * 4;
      unsigned int right = (y * width + (x+1)) * 4;
      unsigned int bottom = ((y-1) * width + x) * 4;

      input[idx] = output[top+2];
      input[idx+1] = output[right+3];
      input[idx+2] = output[bottom];
      input[idx+3] = output[left+1];

      unsigned int vIdx = (y * width + x)*3;
      vertexArray[vIdx+2] = input[idx] + input[idx+1] + input[idx+2] + input[idx+3];
    }
  }
  for (unsigned int y = 2; y < height-3; ++y ) {
    for (unsigned int x = 2; x < width-3; ++x ) {
      unsigned int idx = (y*width+x)*3;
      unsigned int top = ((y+2) * width + x) * 3;
      unsigned int left = (y * width + (x-2)) * 3;
      unsigned int right = (y * width + (x+2)) * 3;
      unsigned int bottom = ((y-2) * width + x) * 3;
      float a[3] = {vertexArray[right] - vertexArray[left], vertexArray[right+1] - vertexArray[left+1], vertexArray[right+2] - vertexArray[left+2]};
      float b[3] = {vertexArray[top] - vertexArray[bottom], vertexArray[top+1] - vertexArray[bottom+1], vertexArray[top+2] - vertexArray[bottom+2]};
      normalArray[idx]   = a[1] *  b[2] - a[2] * b[1];
      normalArray[idx+1] = a[2] *  b[0] - a[0] * b[2];
      normalArray[idx+2] = a[0] *  b[1] - a[1] * b[0];
    }
  }

}

void WaveEffect::draw() {
  glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
//     glRotatef(80, 1.0, 0.0, 0.0);
//     glDrawArrays(GL_POINTS, 0, count);
//      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glColor3f(1.0, 1.0,1.0);
/*  GLfloat lightPOS[] = { sin(frame*0.04) * 3.0, cos(frame*0.04) * 3.0, 1.7, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, lightPOS);*/
   
//   gluLookAt(sin(frame*0.083) * 3.0, cos(frame*0.083) * 3.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);  
//    gluLookAt(2.0, 3.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);  
   
   
   
//    glEnable(GL_LIGHTING);
     
    for (unsigned int i = 0; i < height-1; ++i)
     glDrawElements(GL_TRIANGLE_STRIP, 2*width, GL_UNSIGNED_INT, indices+(2*width*i));
//       glDrawRangeElements(GL_QUAD_STRIP,width*i, width*(i+1), width*2, GL_UNSIGNED_INT, indices);
/*    glBegin(GL_POINTS) ;
      for (unsigned int  i = 0; i < frame; ++i) {
        glVertex3fv(vertexArray+3*i);
//        glVertex3fv(vertexArray+3*i+((3*width)*24));
      }
    glEnd();*/
/*    glColor3f(0.5, 0.5,0.5);
      
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    for (unsigned int i = 0; i < count*3; i+=3)  {
      glVertex3fv(vertexArray+i);
      glVertex3f(vertexArray[i]+normalArray[i], vertexArray[i+1]+normalArray[i+1], vertexArray[i+2]+normalArray[i+2]);
    }
    glEnd();*/
    
}

