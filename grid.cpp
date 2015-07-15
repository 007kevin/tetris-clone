#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h> /* srand, rand */
#include <time.h> /* time */
#include "init.h"
#include "cell.h"
#include "grid.h"

/********************************************
 * Tetromino definitions & colors
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
  //Z-block definition
  { {STARTX + 0, STARTY + 0}, {STARTX + 0, STARTY + 1}, {STARTX + 1, STARTY + 1}, {STARTX + 1, STARTY + 2} },
};

color Colors[][NCOORDS] = {
  //O-block color
  {236,199,127, 0},
  //I-block color
  {236,153,86, 0},
  //J-block color
  {117,166,150, 0},
  //L-block color
  {77,129,183, 0},
  //S-block color
  {183,153,124, 0},
  //T-block color
  {177,137,205, 0},
  //Z-block color
  {127,127,127, 0}
};



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

  set(Colors[index]->r, Colors[index]->g, Colors[index]->b, Colors[index]->a);
}

void Grid::set(Uint8 red, Uint8 grn, Uint8 blu, Uint8 alp){
  //Set cells visible in reference to game piece
  //Note, cell coordinates do not follow the x, y of the
  //window coordinate (i.e cells[4][0] != cell.init(4, 0) 
  for (int i = 0; i < NCOORDS; ++i)
   cells[piece[i].x][piece[i].y].on(red, grn, blu, alp); 
}            

// Get current cell colors; Turn off current cells; Turn on new cell positions; Update piece coordinates
void Grid::move(int d){
  switch (d){
    case DIRECTION_UP:
      if (!isCollision(DIRECTION_UP)){
        Uint8 r, g, b, a;
        for (int i = 0 ; i < NCOORDS; ++i){ 
          r = cells[piece[i].x][piece[i].y].getr();
          g = cells[piece[i].x][piece[i].y].getg();
          b = cells[piece[i].x][piece[i].y].getb();
          a = cells[piece[i].x][piece[i].y].geta();
          cells[piece[i].x][piece[i].y].off();
          piece[i].x -= 1;
        }
        set(r,g,b,a);
      } 
      break;
    case DIRECTION_DOWN:
       if (!isCollision(DIRECTION_DOWN)){
        Uint8 r, g, b, a;
        for (int i = 0 ; i < NCOORDS; ++i){ 
          r = cells[piece[i].x][piece[i].y].getr();
          g = cells[piece[i].x][piece[i].y].getg();
          b = cells[piece[i].x][piece[i].y].getb();
          a = cells[piece[i].x][piece[i].y].geta();
          cells[piece[i].x][piece[i].y].off();
          piece[i].x += 1;
        }
        set(r,g,b,a);
      }  
      break;
    case DIRECTION_LEFT:
        if (!isCollision(DIRECTION_LEFT)){
        Uint8 r, g, b, a;
        for (int i = 0 ; i < NCOORDS; ++i){ 
          r = cells[piece[i].x][piece[i].y].getr();
          g = cells[piece[i].x][piece[i].y].getg();
          b = cells[piece[i].x][piece[i].y].getb();
          a = cells[piece[i].x][piece[i].y].geta();
          cells[piece[i].x][piece[i].y].off();
          piece[i].y -= 1;
        }
        set(r,g,b,a);
      } 
      break;
    case DIRECTION_RIGHT:
        if (!isCollision(DIRECTION_RIGHT)){
        Uint8 r, g, b, a;
        for (int i = 0 ; i < NCOORDS; ++i){ 
          r = cells[piece[i].x][piece[i].y].getr();
          g = cells[piece[i].x][piece[i].y].getg();
          b = cells[piece[i].x][piece[i].y].getb();
          a = cells[piece[i].x][piece[i].y].geta();
          cells[piece[i].x][piece[i].y].off();
          piece[i].y += 1;
        }
        set(r,g,b,a);
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
          if (cells[ln - 1][j].getStatus()){
            Uint8 r, g, b, a;
            r = cells[ln][j].getr();
            g = cells[ln][j].getg();
            b = cells[ln][j].getb();
            a = cells[ln][j].geta();

            cells[ln][j].on(r,g,b,a);
          }
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

















