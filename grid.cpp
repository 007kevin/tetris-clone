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
  if (SDL_RenderDrawRect(gRenderer, &rect) < 0)
    fprintf(stderr, "Could not draw cell: %s\n", SDL_GetError());

  //Reset to default draw colors
  if (SDL_SetRenderDrawColor(gRenderer, dRED, dGREEN, dBLUE, dALPHA) < 0)
    fprintf(stderr, "Could not set draw color: %s\n", SDL_GetError());

}

void Cell::on(){
  r = g = b = a = 0xFF;
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
  // Allocate memory to hold game piece coord
  piece = new coord[4];

  // Allocate memory to hold cells for grid
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

void Grid::load(){
  //All game pieces will consist of NCOORDS cells, 
  /*
  for (int i = 0; i < NCOORDS; ++i){
    piece[i].x = k
  }
  */
  piece[0].x = STARTX + 0; piece[0].y = STARTY + 0;
  piece[1].x = STARTX + 0; piece[1].y = STARTY + 1;
  piece[2].x = STARTX + 1; piece[2].y = STARTY + 0;
  piece[3].x = STARTX + 1; piece[3].y = STARTY + 1;
  set();
}

void Grid::set(){
  //Set cells visible in reference to game piece
  //Note, cell coordinates do not follow the x, y of the
  //window coordinate (i.e cells[4][0] != cell.init(4, 0) 
  for (int i = 0; i < NCOORDS; ++i)
   cells[piece[i].x][piece[i].y].on(); 
}            

void Grid::move(int d){
  switch (d){
    case DIRECTION_UP:
      for (int i = 0 ; i < NCOORDS; ++i){ 
    
      }
      break;
    case DIRECTION_DOWN:
      for (int i = 0 ; i < NCOORDS; ++i){ 

      }
      break;
    case DIRECTION_LEFT:
      for (int i = 0 ; i < NCOORDS; ++i){ 

      }
      break;
    case DIRECTION_RIGHT:
      for (int i = 0 ; i < NCOORDS; ++i){ 

      }
  }
}

bool Grid::isCollision(int d){
  bool flag = false;
  switch (d){
    //Three logical conditions per game piece cell to determine collision:
    //1. Moving in given direction by one cell does go over grid border
    //2. Check that the compared cell status of 1 cell-unit direction over is not its own game piece
    //3. Check the 1 cell-direction over has status set to true (i.e it is an active cell)
    case DIRECTION_UP:
      for (int i = 0; i < NCOORDS; ++i){
        if (piece[i].x - 1 < 0 || (!isGamePiece(piece[i].x - 1, piece[i].y) &&
            cells[piece[i].x - 1][piece[i].y].getStatus())){
          flag = true;
          break;
        }
      }
      break;
    case DIRECTION_DOWN:
      for (int i = 0 ; i < NCOORDS; ++i){ 
        if (piece[i].x + 1 >= NROW || (!isGamePiece(piece[i].x + 1, piece[i].y) &&
            cells[piece[i].x + 1][piece[i].y].getStatus())){
          flag = true;
          break;
        }
      }
      break;
    case DIRECTION_LEFT:
      for (int i = 0 ; i < NCOORDS; ++i){ 
        if (piece[i].y - 1 < 0 || (!isGamePiece(piece[i].x, piece[i].y - 1) &&
            cells[piece[i].x][piece[i].y - 1].getStatus())){
            flag = true;
            printf("piece[%d].x %d\n", i, piece[i].x);
            printf("piece[%d].y %d\n", i, piece[i].y);
            break;
        }
      }
      break;
    case DIRECTION_RIGHT:
      for (int i = 0 ; i < NCOORDS; ++i){ 
        if (piece[i].y + 1 >= NCOL || (!isGamePiece(piece[i].x, piece[i].y + 1) &&
            cells[piece[i].x][piece[i].y + 1].getStatus())){
          flag = true;
          break;
        }
      }
      break;
  }
  return flag;
}

//Check if given coordinated are part of the game piece. Also
//checks if coordinates are outside game borders.
bool Grid::isGamePiece(int r, int c){
  for (int i = 0; i < NCOORDS; ++i)
    if ((piece[i].x == r && piece[i].y == c) ||
        r < 0 || r >= NROW || c < 0 || c >=NCOL)
      return true;
  return false;
}


void Grid::free(){
  //Free cells
  for (int i = 0; i < NROW; ++i)
    delete [] cells[i];
  delete[] cells;

  //Free piece
  delete[] piece;
}

















