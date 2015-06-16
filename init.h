#ifndef GUARD_init_h
#define GUARD_init_h

#include <SDL2/SDL.h>
#include <stdio.h>

#define dRED 0
#define dGREEN 0
#define dBLUE 0
#define dALPHA 0xFF

//Screen dimension constants
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

//Starts up SDL and created window
bool init();

//Frees media and shits down SDL
void close();

//The window we'll be rendering to
extern SDL_Window* gWindow;

//The window renderer
extern SDL_Renderer* gRenderer;

#endif
