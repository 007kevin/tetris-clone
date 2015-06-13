#include <SDL2/SDL.h>
#include <stdio.h>
#include "init.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;


bool init()
{
  //Initialization flag
  bool success = true;

  //Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    success = false;
  }
  else
  {
    //Set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
      fprintf(stderr, "Warining: Linear texture filtering not enabled!");
    }
    
    //Create window
    gWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
      fprintf(stderr, "Window could not be created! SDl Error: %s\n", SDL_GetError() );
      success = false;
    }
    else
    {
      //Create renderer for window
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
      if (gRenderer == NULL)
      {
        fprintf(stderr, "Renderer could not be created! SDL Error %s\n", SDL_GetError());
        success = false;
      }
      else
      {
        //Initialize renderer color
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFf);
      }
    }
  }
  return success;
}

void close()
{
  //Destroy window
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow( gWindow);
  gRenderer = NULL;
  gWindow = NULL;
  

  //Quit SDl subsystems
  SDL_Quit();
}
