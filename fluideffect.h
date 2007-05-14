//
// C++ Interface: fluideffect
//
// Description: 
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FLUIDEFFECT_H
#define FLUIDEFFECT_H


#include <GL/gl.h>
#include "effect.h"

#define IX(i,j) ((i)+(N+2)*(j))

/**
This class simulates a fluid like effect on basis of an algorithm by Jos Stam

	@author Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>
*/
class FluidEffect : public Effect
{
public:
    FluidEffect();
    ~FluidEffect();
    virtual void draw();
    virtual void animate(int t);
  private:
    void free_data ( void );
    void clear_data ( void );
    int  allocate_data ( void );
    void draw_density ( void );
    void get_from_UI ( float * d, float * u, float * v );

    /* methods used to solve the equations */
    void add_source ( int N, float * x, float * s, float dt );
    void set_bnd ( int N, int b, float * x );
    void lin_solve ( int N, int b, float * x, float * x0, float a, float c );
    void diffuse ( int N, int b, float * x, float * x0, float diff, float dt );
    void advect ( int N, int b, float * d, float * d0, float * u, float * v, float dt );
    void project ( int N, float * u, float * v, float * p, float * div );
    void dens_step ( int N, float * x, float * x0, float * u, float * v, float diff, float dt );
    void vel_step ( int N, float * u, float * v, float * u0, float * v0, float visc, float dt );

    int N;
    float dt, diff, visc;
    float force, source;
    
    float * u, * v, * u_prev, * v_prev;
    float * dens, * dens_prev;
    
    int win_x, win_y;
    int mouse_down[3];
    int omx, omy, mx, my;

    std::size_t mw, mh;
    bool *lastMatrix;
    bool *activeMatrix;
    GLfloat *vertexArray;
    GLfloat *velocityArray;
    GLfloat *colorArray;
    std::size_t count;
    std::size_t searchRadius;
};

#endif
