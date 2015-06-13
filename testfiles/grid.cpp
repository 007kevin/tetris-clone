#include <SDL2/SDL.h>
#include <stdio.h>
#include "grid.h"
#include "init.h"


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
  int gridy = SCREEN_HEIGHT/4;
  for (int i = 0; i < NROW; ++i)
    for (int j = 0; j < NCOL; ++j)
      cells[i][j].init(gridx + j*SIZE, gridy + i*SIZE);

}

void Grid::render(){
  SDL_Rect* rectArray;
  for (int i = 0; i < NROW; ++i){
    rectArray = getRow(i);
    SDL_RenderDrawRects(gRenderer, rectArray, NCOL);
    delete [] rectArray;
  }
}

SDL_Rect* Grid::getRow(int n){
  int size = (sizeof(cells[n])/sizeof(Cell));
  SDL_Rect* rectArray = new SDL_Rect[size];
  for (int i = 0; i < size; ++i) 
    rectArray[i] = cells[n][i].getRect();
  return rectArray;
}

void Grid::free(){
  for (int i = 0; i < NROW; i++)
    delete [] cells[i];
  delete cells;
}

/*********************************************
 * Grid Class Definitions
 * ******************************************/

Cell::Cell(){
  rect = {0, 0, 0, 0};
}

Cell::Cell(int x, int y){
  init(x, y);
}

Cell::~Cell(){
  free();
}

void Cell::init(int x, int y){
  rect = {x, y, SIZE, SIZE};
}


void Cell::free(){
  rect = {0, 0, 0, 0};
}

SDL_Rect Cell::getRect(){
  return rect;
}

















