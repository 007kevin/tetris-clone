#include <SDL2/SDL.h>
#include <stdio.h>

LTexture::LTexture(){
  //Initialize
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}

LTexture::~LTexture(){
  //Deallocate
  free();
}

LTexture::free(){
  //Free texture if it exists
  if (mTexture != NULL){
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
  }
}

void render(int x, int y, SDL_Rect* 
