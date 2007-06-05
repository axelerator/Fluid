//
// C++ Implementation: fluideffect
//
// Description:
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "fluideffect.h"
#include "GL/gl.h"
#include <stdlib.h>
#include <math.h>

FluidEffect::FluidEffect(EffectSettings *conf): Effect() {

  dt = conf->getFloatOrDefault("dt", 0.1f);
  diff = conf->getFloatOrDefault("diff",0.0f);
  visc = conf->getFloatOrDefault("visc", 0.0f);
  force = conf->getFloatOrDefault("force", 10.0f);
  source = conf->getFloatOrDefault("source", 100.0f);

  init();

  srcXPos = conf->getIntegerOrDefault("srcx", mw / 2);
  srcYPos = conf->getIntegerOrDefault("srcy", mh / 2);

  if (srcXPos > int(mw)) {
    srcXPos = mw;
  }

  if (srcYPos > int(mh)) {
    srcYPos = mh;
  }
}

FluidEffect::~FluidEffect() {
  free_data();

}

void FluidEffect::init() {
  N = mw = env->getMatrixWidth();
  mh = env->getMatrixHeight();

  win_x = mw;
  win_y = mh;
  count = mw * mh;

  allocate_data();
  clear_data ();

  float xd = 4.0 / N;
  float yd = 2.0 / N;

  for (int i = 0; i < N; ++i ) {
    for (int t = 0; t < N; ++t ) {
      int vidx = (i*N+t)*2;
      vertices[vidx] = t*xd - 2.0 ;
      vertices[vidx+1] =  i * yd - 1.0;
      vertices[vidx+2] =  0.0;
      int cidx = (i*N+t)*3;
      colors[cidx] = 0.2;
      colors[cidx+1] = 0.2;
      colors[cidx+2] = 0.2;
    }
  }

  unsigned int currIdx = 0;
  for (int y = 0; y < (N-1); ++y) {
    for (int x = 0; x < N; ++x) {
      indices[currIdx++] = y * N + x;
      indices[currIdx++] = (y+1) * N + x;
    }
  }
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  
  glVertexPointer(2, GL_FLOAT,0, vertices);
  glColorPointer(3, GL_FLOAT,0,colors);
}

void FluidEffect::draw() {
  glClear ( GL_COLOR_BUFFER_BIT );
  glMatrixMode ( GL_MODELVIEW );
  glLoadIdentity ();
  //draw_density();
  
  for (int i = 0; i < N-1; ++i)
   glDrawElements(GL_QUAD_STRIP, 2*N, GL_UNSIGNED_INT, indices+(2*N*i));
}

void FluidEffect::animate(int t) {
  detector.update();

  get_from_UI ( dens_prev, u_prev, v_prev );
  vel_step ( N, u, v, u_prev, v_prev, visc, dt );
  dens_step ( N, dens, dens_prev, u, v, diff, dt );
  
  unsigned int stride = 3 * N;
  for (int i=0 ; i<N ; i++ ) {
    for (int j=0 ; j<N ; j++ ) {
      colorize(u[IX(i,j)] , v[IX(i,j)], dens[IX(i,j)], colors + j * stride + 3 * i);

     }
  }
}

void FluidEffect::free_data () {
  if (u)
    free(u);
  if (v)
    free(v);
  if (u_prev)
    free(u_prev);
  if (v_prev)
    free(v_prev);
  if (dens)
    free(dens);
  if (dens_prev)
    free(dens_prev);
  delete[] vertices;
  delete[] colors;
  delete[] indices; 
}

void FluidEffect::clear_data ( void ) {
  int i, size=(N+2)*(N+2);

  for ( i=0 ; i<size ; i++ ) {
    u[i] = v[i] = u_prev[i] = v_prev[i] = dens[i] = dens_prev[i] = 0.0f;
  }
}

int FluidEffect::allocate_data ( void ) {
  int size = (N+2)*(N+2);

  u      = (float *) malloc ( size*sizeof(float) );
  v      = (float *) malloc ( size*sizeof(float) );
  u_prev    = (float *) malloc ( size*sizeof(float) );
  v_prev    = (float *) malloc ( size*sizeof(float) );
  dens    = (float *) malloc ( size*sizeof(float) );
  dens_prev  = (float *) malloc ( size*sizeof(float) );

  vertices = new GLfloat[N*N*2];
  colors = new GLfloat[N*N*3];
  indices = new GLuint[(N-1) * (N*2)];

  if ( !u || !v || !u_prev || !v_prev || !dens || !dens_prev ) {
      fprintf ( stderr, "cannot allocate data\n" );
      return ( 0 );
  }

  return ( 1 );
}

void FluidEffect::colorize (float x, float y, float z, float rgb[]) {
  float z05 = z *0.5;
  float l = (z05) / hypot(x,y);
  x *= l;
  y *= l;

  if (z05 > 0.0) {
    rgb[0] = z05;
    rgb[1] = z05 + x;
    rgb[2] = z05 + y; 
  }
  else {
    rgb[0] = rgb[1] = rgb[2] = 0.0;
  }
}

void FluidEffect::get_from_UI(float *d, float *u, float *v) {
  int size = (N+2) * (N+2);

  for (int i = 0; i < size; ++i) {
    u[i] = v[i] = 0.0;
    d[i] = -0.01f;
  }

  for (std::size_t i = 0; i < mh; ++i) {
    for (std::size_t t = 0; t < mw; ++t) {
      if (detector.isActive(t, i)) {
        const float* vel = detector.getVelocity(t, i);
        // directional force
        u[IX(t,i)] = vel[0] * force;
        v[IX(t,i)] = vel[1] * force;
      }
    }
  }

  // constantly add some density
  d[IX(srcXPos, srcYPos)] = source;
}

/* --------------------  SOLVER methods -------------------- */
#define IX(i,j) ((i)+(N+2)*(j))
#define SWAP(x0,x) {float * tmp=x0;x0=x;x=tmp;}
#define FOR_EACH_CELL for ( i=1 ; i<=N ; i++ ) { for ( j=1 ; j<=N ; j++ ) {
#define END_FOR }}

void FluidEffect::add_source ( int N, float * x, float * s, float dt ) {
  int i, size=(N+2)*(N+2);
  for ( i=0 ; i<size ; i++ ) x[i] += dt*s[i];
}

void FluidEffect::set_bnd ( int N, int b, float * x ) {
  int i;

  for ( i=1 ; i<=N ; i++ ) {
    x[IX(0  ,i)] = b==1 ? -x[IX(1,i)] : x[IX(1,i)];
    x[IX(N+1,i)] = b==1 ? -x[IX(N,i)] : x[IX(N,i)];
    x[IX(i,0  )] = b==2 ? -x[IX(i,1)] : x[IX(i,1)];
    x[IX(i,N+1)] = b==2 ? -x[IX(i,N)] : x[IX(i,N)];
  }

  x[IX(0  ,0  )] = 0.5f*(x[IX(1,0  )]+x[IX(0  ,1)]);
  x[IX(0  ,N+1)] = 0.5f*(x[IX(1,N+1)]+x[IX(0  ,N)]);
  x[IX(N+1,0  )] = 0.5f*(x[IX(N,0  )]+x[IX(N+1,1)]);
  x[IX(N+1,N+1)] = 0.5f*(x[IX(N,N+1)]+x[IX(N+1,N)]);
}

void FluidEffect::lin_solve ( int N, int b, float * x, float * x0, float a, float c ) {
  int i, j, k;

  for ( k=0 ; k<20 ; k++ ) {
    FOR_EACH_CELL
      x[IX(i,j)] = (x0[IX(i,j)] + a*(x[IX(i-1,j)]+x[IX(i+1,j)]+x[IX(i,j-1)]+x[IX(i,j+1)]))/c;
    END_FOR
    set_bnd ( N, b, x );
  }
}

void FluidEffect::diffuse ( int N, int b, float * x, float * x0, float diff, float dt ) {
  float a=dt*diff*N*N;
  lin_solve ( N, b, x, x0, a, 1+4*a );
}

void FluidEffect::advect ( int N, int b, float * d, float * d0, float * u, float * v, float dt ) {
  int i, j, i0, j0, i1, j1;
  float x, y, s0, t0, s1, t1, dt0;

  dt0 = dt*N;
  FOR_EACH_CELL
    x = i-dt0*u[IX(i,j)]; y = j-dt0*v[IX(i,j)];
    if (x<0.5f) x=0.5f; if (x>N+0.5f) x=N+0.5f; i0=(int)x; i1=i0+1;
    if (y<0.5f) y=0.5f; if (y>N+0.5f) y=N+0.5f; j0=(int)y; j1=j0+1;
    s1 = x-i0; s0 = 1-s1; t1 = y-j0; t0 = 1-t1;
    d[IX(i,j)] = s0*(t0*d0[IX(i0,j0)]+t1*d0[IX(i0,j1)])+
           s1*(t0*d0[IX(i1,j0)]+t1*d0[IX(i1,j1)]);
  END_FOR
  set_bnd ( N, b, d );
}

void FluidEffect::project ( int N, float * u, float * v, float * p, float * div ) {
  int i, j;

  FOR_EACH_CELL
    div[IX(i,j)] = -0.5f*(u[IX(i+1,j)]-u[IX(i-1,j)]+v[IX(i,j+1)]-v[IX(i,j-1)])/N;
    p[IX(i,j)] = 0;
  END_FOR  
  set_bnd ( N, 0, div ); set_bnd ( N, 0, p );

  lin_solve ( N, 0, p, div, 1, 4 );

  FOR_EACH_CELL
    u[IX(i,j)] -= 0.5f*N*(p[IX(i+1,j)]-p[IX(i-1,j)]);
    v[IX(i,j)] -= 0.5f*N*(p[IX(i,j+1)]-p[IX(i,j-1)]);
  END_FOR
  set_bnd ( N, 1, u ); set_bnd ( N, 2, v );
}

void FluidEffect::dens_step ( int N, float * x, float * x0, float * u, float * v, float diff, float dt ) {
  add_source ( N, x, x0, dt );
  SWAP ( x0, x ); diffuse ( N, 0, x, x0, diff, dt );
  SWAP ( x0, x ); advect ( N, 0, x, x0, u, v, dt );
}

void FluidEffect::vel_step ( int N, float * u, float * v, float * u0, float * v0, float visc, float dt ) {
  add_source ( N, u, u0, dt ); add_source ( N, v, v0, dt );
  SWAP ( u0, u ); diffuse ( N, 1, u, u0, visc, dt );
  SWAP ( v0, v ); diffuse ( N, 2, v, v0, visc, dt );
  project ( N, u, v, u0, v0 );
  SWAP ( u0, u ); SWAP ( v0, v );
  advect ( N, 1, u, u0, u0, v0, dt ); advect ( N, 2, v, v0, u0, v0, dt );
  project ( N, u, v, u0, v0 );
}

