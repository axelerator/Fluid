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

FluidEffect::FluidEffect(): Effect() {
  N = 128;
  dt = 0.1f;
  diff = 0.0f;
  visc = 0.0f;
  force = 10.0f;
  source = 100.0f;

  init();
}

void FluidEffect::init() {
  allocate_data();
  clear_data ();

  mw = env->getMatrixWidth(), mh = env->getMatrixHeight();
  win_x = mw;
  win_y = mh;
  float xd = 4.0 / mw;
  float yd = 2.0 / mh;
  count = mw * mh;
  searchRadius = 10;
  lastMatrix = new bool[count];
  activeMatrix = new bool[count];
  vertexArray = new GLfloat[2 * count];
  colorArray  = new GLfloat[3 * count];
  velocityArray = new GLfloat[2*count];

  for ( std::size_t i = 0; i < mh; ++i ) {
    for ( std::size_t t = 0; t < mw; ++t ) {
      int vidx = ( i*mw+t ) *2;
      vertexArray[vidx] = t*xd - 2.0 ;
      vertexArray[vidx+1] =  i * yd - 1.0 ;
      int cidx = ( i*mw+t ) * 3;
      colorArray[cidx] = 0.3;
      colorArray[cidx+1] = 0.3;
      colorArray[cidx+2] = 0.3;
    }
  }

  glEnableClientState ( GL_VERTEX_ARRAY );
  glEnableClientState ( GL_COLOR_ARRAY );

  glVertexPointer ( 2, GL_FLOAT,0, vertexArray );
  glColorPointer ( 3, GL_FLOAT,0, colorArray );
  glPointSize ( 1.0 );

  memcpy(lastMatrix, env->getMatrix(), count * sizeof(bool));
  memset(velocityArray, 0, 2*count*sizeof(GLfloat));
}

void FluidEffect::draw() {
  glClear ( GL_COLOR_BUFFER_BIT );
  glMatrixMode ( GL_MODELVIEW );
  glLoadIdentity ();
  draw_density();
}

void FluidEffect::animate(int t) {
  int intSize = (count * sizeof(bool)) / sizeof(int);
  int *intMatrix = (int*) env->getMatrix();
  int *actMatrix = (int*) activeMatrix;
  int *lstMatrix = (int*) lastMatrix;

  for (int i = 0; i < intSize; ++i) {
    actMatrix[i] = intMatrix[i] & (~lstMatrix[i]);
  }

  for (std::size_t i = 0; i < count; ++i ) {
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

  memset(velocityArray, 0, 2 * count * sizeof(GLfloat));
  float ssqr = searchRadius * searchRadius;

  for ( std::size_t i = 0; i < mh; ++i ) {
    for ( std::size_t t = 0; t < mw; ++t ) {
      if ( activeMatrix[i*mw+t] ) {
        int vidx = ( i*mw+t ) *2;

        float dir[2] = { searchRadius+1, searchRadius+1 };
        float dist = ssqr;

        std::size_t  xstart = (searchRadius >= t) ? 0 : (t - searchRadius);
        std::size_t  ystart = (searchRadius >= i) ? 0 : (i - searchRadius);

        std::size_t  xend = (t + searchRadius > mw) ? mw : (t + searchRadius);
        std::size_t  yend = (i + searchRadius > mh) ? mh : (i + searchRadius);

        for (std::size_t si = ystart; si < yend; ++si) {
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
        }

        if ( dist < ssqr) {
          velocityArray[vidx]   = dir[0];
          velocityArray[vidx+1] = dir[1];
        }
      }
    }
  }

  memcpy(lastMatrix, env->getMatrix(), count * sizeof(bool));
  get_from_UI ( dens_prev, u_prev, v_prev );
  vel_step ( N, u, v, u_prev, v_prev, visc, dt );
  dens_step ( N, dens, dens_prev, u, v, diff, dt );
}

FluidEffect::~FluidEffect() {
  delete[] lastMatrix;
  delete[] vertexArray;
  delete[] velocityArray;
  delete[] colorArray;
  delete[] activeMatrix;
  free_data();
}

void FluidEffect::free_data ( void ) {
  if ( u )
    free ( u );
  if ( v )
    free ( v );
  if ( u_prev )
    free ( u_prev );
  if ( v_prev )
    free ( v_prev );
  if ( dens )
    free ( dens );
  if ( dens_prev )
    free ( dens_prev );
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

  if ( !u || !v || !u_prev || !v_prev || !dens || !dens_prev ) {
      fprintf ( stderr, "cannot allocate data\n" );
      return ( 0 );
  }

  return ( 1 );
}

/*static void draw_velocity ( void )
{
        int i, j;
        float x, y, h;
 
        h = 1.0f/N;
 
        glColor3f ( 1.0f, 1.0f, 1.0f );
        glLineWidth ( 1.0f );
 
        glBegin ( GL_LINES );
 
                for ( i=1 ; i<=N ; i++ ) {
                        x = (i-0.5f)*h;
                        for ( j=1 ; j<=N ; j++ ) {
                                y = (j-0.5f)*h;
 
                                glVertex2f ( x, y );
                                glVertex2f ( x+u[IX(i,j)], y+v[IX(i,j)] );
                        }
                }
 
        glEnd ();
}
*/

void FluidEffect::draw_density ( void ) {
  int i, j;
  float x, y, h, d00, d01, d10, d11;

  h = 1.0f/N;

  glBegin ( GL_QUADS );

  for ( i=0 ; i<=N ; i++ ) {
    x = (i-0.5f)*h;
    for ( j=0 ; j<=N ; j++ ) {
      y = (j-0.5f)*h;

      d00 = dens[IX(i,j)];
      d01 = dens[IX(i,j+1)];
      d10 = dens[IX(i+1,j)];
      d11 = dens[IX(i+1,j+1)];

      glColor3f ( d00, d00, d00 );
      glVertex2f ( x * 4.0 - 2.0, y * 2.0 - 1.0 );
      glColor3f ( d10, d10, d10 );
      glVertex2f ( (x+h) * 4.0 - 2.0, y  * 2.0 - 1.0);
      glColor3f ( d11, d11, d11 );
      glVertex2f ( (x+h) * 4.0 - 2.0, (y+h)  * 2.0 - 1.0);
      glColor3f ( d01, d01, d01 );
      glVertex2f ( x * 4.0 - 2.0, (y+h)  * 2.0 - 1.0);
    }
  }

  glEnd ();
}

void FluidEffect::get_from_UI(float *d, float *u, float *v) {
  int size = (N+2) * (N+2);

  for (int i = 0; i < size; ++i) {
    u[i] = v[i] = 0.0;
    d[i] = -0.01f;
  }

  int velIdx;
  for (std::size_t i = 0; i < mh; ++i) {
    for (std::size_t t = 0; t < mw; ++t) {
      if (activeMatrix[i*mw+t]) {
        velIdx = (t * mh + i) * 2;
        // directional force
        u[IX(t,i)] = velocityArray[velIdx] * force;
        v[IX(t,i)] = velocityArray[velIdx+1] * force;
      }
    }
  }

  // constantly add some density
  d[IX(mh / 2, mw / 2)] = source;

//     if ( !mouse_down[0] && !mouse_down[2] )
//         return;
// 
//     i = (int)((       mx /(float)win_x)*N+1);
//     j = (int)(((win_y-my)/(float)win_y)*N+1);
// 
//     if ( i<1 || i>N || j<1 || j>N )
//         return;
// 
//     if ( mouse_down[0] ) {
//         u[IX(i,j)] = force * (mx-omx);
//         v[IX(i,j)] = force * (omy-my);
//     }
// 
//     if ( mouse_down[2] ) {
//         d[IX(i,j)] = source;
//     }
// 
//     omx = mx;
//     omy = my;
// 
//     return;
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

