#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <stdlib.h> /* srand, rand */
#include <time.h> /* time */
#include "init.h"
#include "cell.h"
#include "grid.h"

/********************************************
 * Tetromino definitions & colors
 * ******************************************/
tetro Tetrominoes[] = { 
  //O-block definition
  { 
    // coord pos[NCOORDS]
    { {0, 0}, {0, 1}, {1, 0}, {1, 1} },

    //coord rotations[NCOORDS][NCOORDS]
    {
      { {0, 0}, {0, 1}, {1, 0}, {1, 1} },
      { {0, 0}, {0, 1}, {1, 0}, {1, 1} },
      { {0, 0}, {0, 1}, {1, 0}, {1, 1} },
      { {0, 0}, {0, 1}, {1, 0}, {1, 1} }
    },
    //color Color
    {236,199,127, 0} 
  },
  //I-block definition
  { 
    { {1, 0}, {1, 1}, {1, 2}, {1, 3} },
    {
      { {1, 0}, {1, 1}, {1, 2}, {1, 3} },
      { {0, 1}, {1, 1}, {2, 1}, {3, 1} },
      { {1, 0}, {1, 1}, {1, 2}, {1, 3} },
      { {0, 1}, {1, 1}, {2, 1}, {3, 1} },
    },
    {236,153,86, 0}
  },
  //J-block definition
  { 
    { {0, 0}, {1, 0}, {1, 1}, {1, 2} },
    {
      { {0, 0}, {1, 0}, {1, 1}, {1, 2} },
      { {2, 0}, {0, 1}, {1, 1}, {2, 1} },
      { {2, 2}, {2, 1}, {1, 1}, {0, 1} },
      { {0, 2}, {1, 2}, {1, 1}, {1, 0} }
    },
    {117,166,150, 0} 
  },
  //L-block definition
  { 
    { {1, 0}, {1, 1}, {1, 2}, {0, 2} },
    {
      { {1, 0}, {1, 1}, {1, 2}, {0, 2} },
      { {0, 1}, {1, 1}, {1, 2}, {0, 2} },
      { {1, 0}, {1, 1}, {1, 2}, {0, 2} },
      { {1, 0}, {1, 1}, {1, 2}, {0, 2} }
    },
    {77,129,183, 0}  
  },
  //S-block definition
  { 
    { {1, 0}, {1, 1}, {0, 1}, {0, 2} },
    {
      { {1, 0}, {1, 1}, {0, 1}, {0, 2} },
      { {1, 0}, {1, 1}, {0, 1}, {0, 2} },
      { {1, 0}, {1, 1}, {0, 1}, {0, 2} },
      { {1, 0}, {1, 1}, {0, 1}, {0, 2} }
    },
    {183,153,124, 0} 
  },
  //T-block definition
  { 
    { {1, 0}, {1, 1}, {0, 1}, {1, 2} },
    {
      { {1, 0}, {1, 1}, {0, 1}, {1, 2} },
      { {1, 0}, {1, 1}, {0, 1}, {1, 2} },
      { {1, 0}, {1, 1}, {0, 1}, {1, 2} },
      { {1, 0}, {1, 1}, {0, 1}, {1, 2} }
    },
    {177,137,205, 0} 
  },
  //Z-block definition
  { 
    { {0, 0}, {0, 1}, {1, 1}, {1, 2} },
    {
      { {0, 0}, {0, 1}, {1, 1}, {1, 2} },
      { {0, 0}, {0, 1}, {1, 1}, {1, 2} },
      { {0, 0}, {0, 1}, {1, 1}, {1, 2} },
      { {0, 0}, {0, 1}, {1, 1}, {1, 2} }
    },
    {127,127,127, 0} 
  }
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

  // Initialize game piece with NULL
  

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
  piece = Tetrominoes[index];

  //Adjust tetromino starting positions to defined starting positions
  for (int i = 0; i < NCOORDS; ++i){
    piece.pos[i].x = STARTX + piece.pos[i].x;
    piece.pos[i].y = STARTY + piece.pos[i].y;
  }

  set(piece.Color.r, piece.Color.g,
      piece.Color.b, piece.Color.a);
}

void Grid::set(Uint8 red, Uint8 grn, Uint8 blu, Uint8 alp){
  //Set cells visible in reference to game piece
  //Note, cell coordinates do not follow the x, y of the
  //window coordinate (i.e cells[4][0] != cell.init(4, 0) 
  for (int i = 0; i < NCOORDS; ++i)
   cells[piece.pos[i].x][piece.pos[i].y].on(red, grn, blu, alp); 
}            

// Get current cell colors; Turn off current cells; Turn on new cell positions; Update piece coordinates
void Grid::move(int d){
  switch (d){
    case DIRECTION_UP:
      if (!isCollision(DIRECTION_UP)){
        Uint8 r, g, b, a;
        for (int i = 0 ; i < NCOORDS; ++i){ 
          r = cells[piece.pos[i].x][piece.pos[i].y].getr();
          g = cells[piece.pos[i].x][piece.pos[i].y].getg();
          b = cells[piece.pos[i].x][piece.pos[i].y].getb();
          a = cells[piece.pos[i].x][piece.pos[i].y].geta();
          cells[piece.pos[i].x][piece.pos[i].y].off();
          piece.pos[i].x -= 1;
        }
        set(r,g,b,a);
      } 
      break;
    case DIRECTION_DOWN:
       if (!isCollision(DIRECTION_DOWN)){
        Uint8 r, g, b, a;
        for (int i = 0 ; i < NCOORDS; ++i){ 
          r = cells[piece.pos[i].x][piece.pos[i].y].getr();
          g = cells[piece.pos[i].x][piece.pos[i].y].getg();
          b = cells[piece.pos[i].x][piece.pos[i].y].getb();
          a = cells[piece.pos[i].x][piece.pos[i].y].geta();
          cells[piece.pos[i].x][piece.pos[i].y].off();
          piece.pos[i].x += 1;
        }
        set(r,g,b,a);
      }  
      break;
    case DIRECTION_LEFT:
        if (!isCollision(DIRECTION_LEFT)){
        Uint8 r, g, b, a;
        for (int i = 0 ; i < NCOORDS; ++i){ 
          r = cells[piece.pos[i].x][piece.pos[i].y].getr();
          g = cells[piece.pos[i].x][piece.pos[i].y].getg();
          b = cells[piece.pos[i].x][piece.pos[i].y].getb();
          a = cells[piece.pos[i].x][piece.pos[i].y].geta();
          cells[piece.pos[i].x][piece.pos[i].y].off();
          piece.pos[i].y -= 1;
        }
        set(r,g,b,a);
      } 
      break;
    case DIRECTION_RIGHT:
        if (!isCollision(DIRECTION_RIGHT)){
        Uint8 r, g, b, a;
        for (int i = 0 ; i < NCOORDS; ++i){ 
          r = cells[piece.pos[i].x][piece.pos[i].y].getr();
          g = cells[piece.pos[i].x][piece.pos[i].y].getg();
          b = cells[piece.pos[i].x][piece.pos[i].y].getb();
          a = cells[piece.pos[i].x][piece.pos[i].y].geta();
          cells[piece.pos[i].x][piece.pos[i].y].off();
          piece.pos[i].y += 1;
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
        if (piece.pos[i].x - 1 < 0 || (!isGamePiece(piece.pos[i].x - 1, piece.pos[i].y) &&
            cells[piece.pos[i].x - 1][piece.pos[i].y].getStatus())){
          flag = true;
          break;
        }
      }
      break;
    case DIRECTION_DOWN:
      for (int i = 0 ; i < NCOORDS; ++i){ 
        if (piece.pos[i].x + 1 >= NROW || (!isGamePiece(piece.pos[i].x + 1, piece.pos[i].y) &&
            cells[piece.pos[i].x + 1][piece.pos[i].y].getStatus())){
          flag = true;
          break;
        }
      }
      break;
    case DIRECTION_LEFT:
      for (int i = 0 ; i < NCOORDS; ++i){ 
        if (piece.pos[i].y - 1 < 0 || (!isGamePiece(piece.pos[i].x, piece.pos[i].y - 1) &&
            cells[piece.pos[i].x][piece.pos[i].y - 1].getStatus())){
            flag = true;
            break;
        }
      }
      break;
    case DIRECTION_RIGHT:
      for (int i = 0 ; i < NCOORDS; ++i){ 
        if (piece.pos[i].y + 1 >= NCOL || (!isGamePiece(piece.pos[i].x, piece.pos[i].y + 1) &&
            cells[piece.pos[i].x][piece.pos[i].y + 1].getStatus())){
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
    if (piece.pos[i].x == r && piece.pos[i].y == c)
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

void Grid::rotate(){
  const char *center = "Hello";
  printf("%s\n", center);
}


void Grid::free(){
  //Free cells
  for (int i = 0; i < NROW; ++i)
    delete [] cells[i];
  delete[] cells;

}

















