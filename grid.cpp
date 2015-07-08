#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h> /* srand, rand */
#include <time.h> /* time */
#include "grid.h"
#include "init.h"

/********************************************
 * Tetromino definitions
 * ******************************************/
coord Tetrominoes[][NCOORDS] = { 
  //O-block definition
  { {STARTX + 0, STARTY + 0}, {STARTX + 0, STARTY + 1}, {STARTX + 1, STARTY + 0}, {STARTX + 1, STARTY + 1} },
  //I-block definition
  { {STARTX + 0, STARTY + 0}, {STARTX + 0, STARTY + 1}, {STARTX + 0, STARTY + 2}, {STARTX + 0, STARTY + 3} },
  //J-block definition
  { {STARTX + 0, STARTY + 0}, {STARTX + 1, STARTY + 0}, {STARTX + 1, STARTY + 1}, {STARTX + 1, STARTY + 2} },
  //L-block definition
  { {STARTX + 1, STARTY + 0}, {STARTX + 1, STARTY + 1}, {STARTX + 1, STARTY + 2}, {STARTX + 0, STARTY + 2} },
  //S-block definition
  { {STARTX + 1, STARTY + 0}, {STARTX + 1, STARTY + 1}, {STARTX + 0, STARTY + 1}, {STARTX + 0, STARTY + 2} },
  //T-block definition
  { {STARTX + 1, STARTY + 0}, {STARTX + 1, STARTY + 1}, {STARTX + 0, STARTY + 1}, {STARTX + 1, STARTY + 2} },
  //T-block definition
  { {STARTX + 0, STARTY + 0}, {STARTX + 0, STARTY + 1}, {STARTX + 1, STARTY + 1}, {STARTX + 1, STARTY + 2} },
};

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
  // Initialize random seed for randomly selecting tetromino definitions
  srand(time(NULL));

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
/*
  cells[10][2].on();
  cells[10][3].on();
  cells[10][4].on();
  cells[10][5].on();
*/
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
  //Subtraction (-2) and adding (+3) factors for 1 pixel
  //space between cells and border
  SDL_Rect outline = {SCREEN_WIDTH/4 - 2, SCREEN_HEIGHT/8 - 2, NCOL*SIZE + 3, NROW*SIZE + 3};

  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderDrawRect(gRenderer, &outline);
  
  //Render onto window
  SDL_RenderPresent(gRenderer);
}

void Grid::load(){
  //Generate random number corresponding to number to tetromino definitions
  int index = rand() % (sizeof(Tetrominoes)/sizeof(*Tetrominoes));

  //Copy tetromino definition to piece array
  //Note: All game pieces will consist of NCOORDS cells, 
  for (int i = 0; i < NCOORDS; ++i){
    piece[i].x = Tetrominoes[index][i].x;
    piece[i].y = Tetrominoes[index][i].y;
  }

  set();
}

void Grid::set(){
  //Set cells visible in reference to game piece
  //Note, cell coordinates do not follow the x, y of the
  //window coordinate (i.e cells[4][0] != cell.init(4, 0) 
  for (int i = 0; i < NCOORDS; ++i)
   cells[piece[i].x][piece[i].y].on(); 
}            

// Turn off current cells; Turn on new cell positions; Update piece coordinates
void Grid::move(int d){
  switch (d){
    case DIRECTION_UP:
      if (!isCollision(DIRECTION_UP)){
        for (int i = 0 ; i < NCOORDS; ++i){ 
          cells[piece[i].x][piece[i].y].off();
          piece[i].x -= 1;
        }
        set();
      } 
      break;
    case DIRECTION_DOWN:
       if (!isCollision(DIRECTION_DOWN)){
        for (int i = 0 ; i < NCOORDS; ++i){ 
          cells[piece[i].x][piece[i].y].off();
          piece[i].x += 1;
        }
        set();
      }  
      break;
    case DIRECTION_LEFT:
        if (!isCollision(DIRECTION_LEFT)){
        for (int i = 0 ; i < NCOORDS; ++i){ 
          cells[piece[i].x][piece[i].y].off();
          piece[i].y -= 1;
        }
        set();
      } 
      break;
    case DIRECTION_RIGHT:
        if (!isCollision(DIRECTION_RIGHT)){
        for (int i = 0 ; i < NCOORDS; ++i){ 
          cells[piece[i].x][piece[i].y].off();
          piece[i].y += 1;
        }
        set();
      }  
      break;
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
    if (piece[i].x == r && piece[i].y == c)
      return true;
  return false;
}

//Scans grid for any complete lines. If found, delete then 
//shift the blocks above down. 
void Grid::shift(){
  for (int i = 0; i < NROW; ++i){
    //Flag for complete line
    bool line = true;
    for (int j = 0; j < NCOL; ++j)
      if (!cells[i][j].getStatus())
        line = false;
    

    //Starting from complete line to first row,
    //shift blocks down
    if (line && i > 0){
      for (int ln = i; ln > 0; --ln){
        for (int j = 0; j < NCOL; ++j){
          //Get state of cell above
          if (cells[ln - 1][j].getStatus())
            cells[ln][j].on();
          else
            cells[ln][j].off();
        }
      }
    }
  }

}


void Grid::free(){
  //Free cells
  for (int i = 0; i < NROW; ++i)
    delete [] cells[i];
  delete[] cells;

  //Free piece
  delete[] piece;
}

















