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
#include <math.h>
#include <time.h>
#include <signal.h>

#include "effectmanager.h"
#include "environment.h"

bool done = 0; // If true, program will terminate
int cursor[2]; // Stores position of the mousecursor in matrix coordinates

bool initOpenGL(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D ( -2.0, 2.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    return (glGetError() == GL_NO_ERROR);
}

int createWindow(std::string title, int width, int height, bool fullscreen) {

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("error: %s", SDL_GetError());
        return 0;
    }

    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    // matrix = (bool*)malloc(sizeof(bool)*M_WIDTH*M_HEIGHT);
    Uint32 flags = SDL_OPENGL | ( fullscreen ? SDL_FULLSCREEN : 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if(SDL_SetVideoMode(width, height, info->vfmt->BitsPerPixel, flags ) == 0) {
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
            if ( env->getMousesimulation() ) {

              cursor[0] = (int) (((float)event.motion.x / env->getScreenWidth())*env->getMatrixWidth());
              cursor[1] = env->getMatrixHeight() - (int) (((float)(event.motion.y) /
                          env->getScreenHeight())*env->getMatrixHeight());

              bool *matrix = env->getMatrix();
              memset(matrix, 0, env->getMatrixHeight() * env->getMatrixWidth());
              int radius =env->getMouseRadius() ;
              for (int i = -radius; i < radius; ++i)
                for (int t = -radius; t < radius; ++t) {
                  int index[2] = { cursor[0] + i, cursor[1] + t };
                  if ((index[0] > 0) && (index[1] > 0) && (index[0] < env->getMatrixWidth()) && 
                      (index[1] < env->getMatrixHeight())) {

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
            case SDLK_RIGHT: 
              EffectManager::getInstance()->nextEffect(); 
              break;

            case SDLK_LEFT:
              EffectManager::getInstance()->previousEffect();
              break;

            case SDLK_q: 
            case SDLK_ESCAPE:
              done = true; 
              break;

            default:
              ;
          }
          break;
        case SDL_QUIT:
            done = true;
        }
    }
}

void SignalHandler(int sig) {
  signal(sig, SIG_IGN);

  switch (sig) {
    case SIGINT: 
      std::cout << "SIGINT catched, exiting..." << std::endl;
      break;

    default: 
      std::cout << "Unknown signal catched, exiting..." << std::endl;
      break;
  }

  done = true;
}

int main(int argc, char *argv[]) {
    std::cout << "Initializing Effect Master 3000..." << std::endl;

    signal(SIGINT, SignalHandler);

    EffectManager *mgr = EffectManager::getInstance();
    Environment *env = mgr->getEnvironment();
    env->loadConfig("effectmaster.conf");

    if (!createWindow("Effect Master 3000", env->getScreenWidth(), env->getScreenHeight(), env->isFullscreen()) ||
            !initOpenGL(env->getScreenWidth(), env->getScreenHeight())) {
        std::cout << "Error creating window!" << std::endl;
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

    std::cout << "Initialization done!" << std::endl;

    mgr->init();

    while(!done) {
        passedMS = SDL_GetTicks() - frameStart;
        frameStart = SDL_GetTicks();
        mgr->animate(passedMS);
        mgr->draw();
        SDL_GL_SwapBuffers();
        userInput();
        rest = msPerFrame - (SDL_GetTicks() - frameStart);
        if (rest < 0 )
            rest = 0;
        SDL_Delay(rest);
    }

    std::cout << "Shutting down Effect Master 3000..." << std::endl;
    delete env;
    delete mgr;

    return 0;
}
