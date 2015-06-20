#include <SDL2/SDL.h>
#include <stdio.h>
#include "grid.h"
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
  //Set cell position
  rect = {x, y, SIZE, SIZE};

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
  if (SDL_RenderDrawRect(gRenderer, &rect) < 0)
    fprintf(stderr, "Could not draw cell: %s\n", SDL_GetError());

  //Reset to default draw colors
  if (SDL_SetRenderDrawColor(gRenderer, dRED, dGREEN, dBLUE, dALPHA) < 0)
    fprintf(stderr, "Could not set draw color: %s\n", SDL_GetError());

}

void Cell::on(){
  r = g = b = a = 0xFF;
}

void Cell::off(){
  r = dRED;
  g = dGREEN;
  b = dBLUE;
  a = dALPHA;
}

/*********************************************
 * Grid Class Definitions
 * ******************************************/

Grid::Grid(){
  init();
}

Grid::~Grid(){
  //Deallocate
  free();
}

void Grid::init(){
  cells = new Cell*[NROW];
  for (int i = 0; i < NROW; i++){
    cells[i] = new Cell[NCOL];
  }
  int gridx = SCREEN_WIDTH/4;
  int gridy = SCREEN_HEIGHT/8;
  for (int i = 0; i < NROW; ++i)
    for (int j = 0; j < NCOL; ++j)
      cells[i][j].init(gridx + j*SIZE, gridy + i*SIZE);

}

void Grid::render(){
  //Default rendering color when clearing renderer
  SDL_SetRenderDrawColor(gRenderer, dRED, dGREEN, dBLUE, dALPHA);
  SDL_RenderClear(gRenderer);
 
  //Render NROW x NCOL cells table
  for (int i = 0; i < NROW; ++i){
    for (int j = 0; j < NCOL; ++j){
      cells[i][j].render();
    }
  }

  //Draw border around cells
  SDL_Rect outline = {SCREEN_WIDTH/4, SCREEN_HEIGHT/8, NCOL*SIZE, NROW*SIZE};

  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderDrawRect(gRenderer, &outline);
  
  //Render onto window
  SDL_RenderPresent(gRenderer);
}

void Grid::free(){
  for (int i = 0; i < NROW; ++i)
    delete [] cells[i];
  delete[] cells;
}

















