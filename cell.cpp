#include <SDL2/SDL.h>
#include <stdio.h>
#include "cell.h"
#include "init.h"
/*********************************************
 * Cell Class Definitions
 * ******************************************/

Cell::Cell(){
  rect = {0, 0, 0, 0};
}

Cell::Cell(int x, int y){
  //Set cell position
  init(x, y);
}

Cell::~Cell(){
  free();
}

void Cell::init(int x, int y){
  //Set cell position. Subtract by 1 to allow 1 pixel space 
  //between cells
  rect = {x, y, SIZE - 1, SIZE - 1};

  //Set initial status of cell to off
  status = false;

  //Set default colors of cell (current grid background)
  r = dRED;
  g = dGREEN;
  b = dBLUE;
  a = dALPHA;
}


void Cell::free(){
  rect = {0, 0, 0, 0};
}


void Cell::render(){

  //Set cell's draw colors
  if (SDL_SetRenderDrawColor(gRenderer, r, g, b, a) < 0)
    fprintf(stderr, "Could not set draw color: %s\n", SDL_GetError());

  //Render with draw colors
  if (SDL_RenderFillRect(gRenderer, &rect) < 0)
    fprintf(stderr, "Could not draw cell: %s\n", SDL_GetError());

  //Reset to default draw colors
  if (SDL_SetRenderDrawColor(gRenderer, dRED, dGREEN, dBLUE, dALPHA) < 0)
    fprintf(stderr, "Could not set draw color: %s\n", SDL_GetError());

}

void Cell::on(Uint8 red, Uint8 grn, Uint8 blu, Uint8 alp ){
  r = red; 
  g = grn;
  b = blu;
  a = alp;
  status = true;
}

void Cell::off(){
  r = dRED;
  g = dGREEN;
  b = dBLUE;
  a = dALPHA;
  status = false;
}

bool Cell::getStatus(){
  return status;
}

Uint8 Cell::getr() {return r;}
Uint8 Cell::getg() {return g;}
Uint8 Cell::getb() {return b;}
Uint8 Cell::geta() {return a;}



