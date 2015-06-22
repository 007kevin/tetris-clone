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

    //Grid
    //Note: Object destructor is automatically called
    //when object gets out of scope
    Grid tetris;
    tetris.load();
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
        //User presses a key
        else if (e.type == SDL_KEYDOWN)
        {
          //Move game piece
          switch (e.key.keysym.sym)
          {
            case SDLK_UP:
              printf("UP %d\n", tetris.isCollision(DIRECTION_UP));
              break;
            case SDLK_DOWN:
              printf("DOWN %d\n", tetris.isCollision(DIRECTION_DOWN));
              break;
            case SDLK_LEFT:
              printf("LEFT %d\n", tetris.isCollision(DIRECTION_LEFT));
              break;
            case SDLK_RIGHT:
              printf("RIGHT %d\n", tetris.isCollision(DIRECTION_RIGHT));
              break;
          }
        }
      }
      
      tetris.render();

    }

  }

  //Free resources and close SDL
  close();
  return 0;
}

