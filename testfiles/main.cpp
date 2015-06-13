/* By Kevin Kim June 10 2015 */

//Using SDL, standard IO, and strings
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include "init.h"
#include "grid.h"

int main(int argc, char* args[])
{
  //Start up SDL and create window
  if (!init())
  {
    printf("Failed to initialize!\n");
  }
  else
  {
    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //Grid object
    Grid tetris;

    //While application is running
    while(!quit)
    {
      //Handle events on queue
      while(SDL_PollEvent(&e) != 0)
      {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
          quit = true;
        }
      }
      SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
      SDL_RenderClear(gRenderer);

      SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
      tetris.render(); 
      SDL_RenderPresent(gRenderer);
    }


  }

  //Free resources and close SDL
  close();
  return 0;
}

