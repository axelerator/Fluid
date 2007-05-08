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
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "effectmanager.h"
#include "environment.h"
#include <time.h>
int done = 0; // if true program will terminate

bool initOpenGL(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, 2.6667, 1.0, 100.0);
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
      static SDL_Event event;
         while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                case SDL_QUIT:
                done = 1;
            }
        } 
}


int main(int argc, char *argv[]) {
    std::cout << "Initializing Effect Master 3000...";

    Environment *env = EffectManager::getInstance()->getEnvironment();
    env->loadConfig("dummy path");

    if (!createWindow("Effect Master 3000", env->getScreenWidth(), env->getScreenHeight()) ||
            !initOpenGL(env->getScreenWidth(), env->getScreenHeight())) {
        std::cout << "error!" << std::endl;
        exit(1);
    }
    
    int fps = env->getFps();
    int msPerFrame = 1000/fps;
    int passedMS = msPerFrame;
    unsigned int frameStart = 0;
    int rest = 0;
    while(!done) {
        passedMS = frameStart - SDL_GetTicks();
        frameStart = SDL_GetTicks();
        EffectManager::getInstance()->animate(passedMS);
        EffectManager::getInstance()->draw();
        SDL_GL_SwapBuffers();
        userInput();
        rest = msPerFrame - (SDL_GetTicks() - frameStart);
        if (rest < 0 ) rest = 0;
        SDL_Delay(rest);
    }
    return 0;
}
