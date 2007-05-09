//
// C++ Implementation: main
//
// Description:
//
//
// Author: Axel Tetzlaff & Timo B. Hübel <axel.tetzlaff@gmx.de / t.h@gmx.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "effectmanager.h"
#include "environment.h"
#include <math.h>
#include <time.h>
int done = 0; // if true program will terminate
bool interactive = true; // in interactive mode matrix is alter by mouse movement
int cursor[2]; // stores position of the mousecursor in matrix coordinates

bool initOpenGL(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D ( -2.0, 2.0, -1.0, 1.0);
    return (glGetError() == GL_NO_ERROR);
}

int createWindow(std::string title, int width, int height) {

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("error: %s", SDL_GetError());
        return 0;
    }

    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    // matrix = (bool*)malloc(sizeof(bool)*M_WIDTH*M_HEIGHT);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if(SDL_SetVideoMode(width, height, info->vfmt->BitsPerPixel, SDL_OPENGL) == 0) {
        printf("error: %s", SDL_GetError());
        return 0;
    }

    SDL_WM_SetCaption("Effect Master 3000", "Effect Master 3000");
    SDL_ShowCursor(1);
    return 1;
}

void userInput() {
    Environment *env = EffectManager::getInstance()->getEnvironment();
    static SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_MOUSEMOTION:
            if ( interactive ) {

              cursor[0] = (int) (((float)event.motion.x / env->getScreenWidth())*env->getMatrixWidth());
              cursor[1] = env->getMatrixHeight() - (int) (((float)(event.motion.y) / env->getScreenHeight())*env->getMatrixHeight());
              
              bool *matrix = env->getMatrix();
              memset(matrix, 0, env->getMatrixHeight() * env->getMatrixWidth());
              int radius = 10;
              for (int i = -radius; i < radius; ++i)
                for (int t = -radius; t < radius; ++t) {
                  int index[2] = { cursor[0] + i, cursor[1] + t };
                  if ( ( index[0] > 0 ) && ( index[1] > 0 ) && ( index[0] < env->getMatrixWidth() ) && ( index[1] < env->getMatrixHeight() )) {
                      float d[2] = { cursor[0] - index[0], cursor[1] - index[1] }; 
                      float distance = sqrt(d[0]*d[0] + d[1] * d[1]);
                      if (distance < radius)
                        matrix[index[1]*env->getMatrixWidth() + index[0]] = 1;
                  }
                }
            }
            break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_RIGHT: EffectManager::getInstance()->nextEffect(); break;
            case SDLK_LEFT: EffectManager::getInstance()->previousEffect(); break;
            case SDLK_q: 
            case SDLK_ESCAPE: 
                              done = 1; break;
            default:;
          }
          break;
        case SDL_QUIT:
            done = 1;
        }
    }
}


int main(int argc, char *argv[]) {
    std::cout << "Initializing Effect Master 3000...";
    EffectManager *mgr = EffectManager::getInstance();
    Environment *env = mgr->getEnvironment();
    env->loadConfig("effectmaster.conf");

    if (!createWindow("Effect Master 3000", env->getScreenWidth(), env->getScreenHeight()) ||
            !initOpenGL(env->getScreenWidth(), env->getScreenHeight())) {
        std::cout << "error!" << std::endl;
        exit(1);
    }
    GLenum err = glewInit();
    if (GLEW_OK != err) {/* Problem: glewInit failed, something is seriously wrong. */
      std::cout << "Error initializing GLEW: " << glewGetErrorString(err)  << std::endl;
      exit(1);
    }
    int fps = env->getFps();
    int msPerFrame = 1000/fps;
    int passedMS = msPerFrame;
    unsigned int frameStart = 0;
    int rest = 0;
    mgr->init();
    while(!done) {
        passedMS = SDL_GetTicks() - frameStart;
        frameStart = SDL_GetTicks();
        EffectManager::getInstance()->animate(passedMS);
        EffectManager::getInstance()->draw();
        SDL_GL_SwapBuffers();
        userInput();
        rest = msPerFrame - (SDL_GetTicks() - frameStart);
        if (rest < 0 )
            rest = 0;
        SDL_Delay(rest);
    }
    return 0;
}
