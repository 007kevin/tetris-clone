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
    { {0, 0}, {0, 1}, 
      {1, 0}, {1, 1} },
    //color Color
    {236,199,127, 0},
    // corner of box containing tetromino for rotation
    {0, 0},
    // length of box
    2
  },
  //I-block definition
  { 
    { {1, 0}, {1, 1},
      {1, 2}, {1, 3} },
    {236,153,86, 0},
    {0, 0},
    4
  },
  //J-block definition
  { 
    { {0, 0}, {1, 0},
      {1, 1}, {1, 2} },
    {117,166,150, 0},
    {0, 0},
    3
  },
  //L-block definition
  { 
    { {1, 0}, {1, 1},
      {1, 2}, {0, 2} },
    {77,129,183, 0},  
    {0, 0},
    3
  },
  //S-block definition
  { 
    { {1, 0}, {1, 1},
      {0, 1}, {0, 2} },
    {183,153,124, 0},
    {0, 0},
    3
  },
  //T-block definition
  { 
    { {1, 0}, {1, 1},
      {0, 1}, {1, 2} },
    {177,137,205, 0},
    {0, 0},
    3
  },
  //Z-block definition
  { 
    { {0, 0}, {0, 1},
      {1, 1}, {1, 2} },
    {127,127,127, 0},
    {0, 0},
    3
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

  //Offset starting positions specified be STARTX and STARTY
  for (int i = 0; i < NCOORDS; ++i){
    piece.pos[i].x = STARTX + piece.pos[i].x;
    piece.pos[i].y = STARTY + piece.pos[i].y;
  }
  piece.bcorner.x = STARTX + piece.bcorner.x;
  piece.bcorner.y = STARTY + piece.bcorner.y;

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
// Return bool whether move was successful
bool Grid::move(int d){
  // Temporarily store coordinates of tetromino to be moved if there is no collision
  coord tmp[NCOORDS], tmpcorner = piece.bcorner;
  memcpy(tmp, piece.pos, sizeof(piece.pos));


  switch (d){
    case DIRECTION_UP:
      // Set tmp coordinates to where moved piece would be
      for (int i = 0; i < NCOORDS; ++i)
        tmp[i].x--;
      tmpcorner.x--;
      break;
    case DIRECTION_DOWN:
      for (int i = 0; i < NCOORDS; ++i)
        tmp[i].x++;
      tmpcorner.x++;
      break;
    case DIRECTION_LEFT:
      for (int i = 0; i < NCOORDS; ++i)
        tmp[i].y--;
      tmpcorner.y--;
      break;
    case DIRECTION_RIGHT:
      for (int i = 0; i < NCOORDS; ++i)
        tmp[i].y++;
      tmpcorner.y++;
      break;
  }

  if (!isCollision(tmp)){
    for (int i = 0 ; i < NCOORDS; ++i){ 
      cells[piece.pos[i].x][piece.pos[i].y].off();
    }
    memcpy(piece.pos, tmp, sizeof(piece.pos));
    piece.bcorner = tmpcorner;
    color *col = &piece.Color;
    set(col->r,col->g,col->b,col->a);
    return true;
  }  
  else
    return false;
}

// To check for collision, must check the new position does not contain
// coordinates outside the tetris grid along with any overlap with any 
// non-current tetromino cell
bool Grid::isCollision(const coord* position){
  bool flag = false;
  for (int i = 0; i < NCOORDS; ++i){
    const coord* tmp = &position[i];
    if (tmp->x < 0 || tmp->x >= NROW || tmp->y < 0 || tmp->y >= NCOL ||
        (!isGamePiece(tmp->x, tmp->y) && cells[tmp->x][tmp->y].getStatus())){
      flag = true;
      break; 
    }
  }
  return flag;
}

//Check if given coordinated are part of the game piece.
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

/* Transpose and change columns to rotate 90 deg, or change rows to rotate -90 deg */
void Grid::rotate(){
  coord tmp[NCOORDS], tmpcorner = piece.bcorner;
  int indx = 0;

  //Transpose & swap columns
  int sx = piece.bcorner.x, sy = piece.bcorner.y;
  int blen = piece.blength;
  for (int i = 0; i < blen; ++i)
    for (int j = 0; j < blen; ++j){
      if (isGamePiece(sx + i,sy + j))
        tmp[indx++] = {sx + j, sy + blen - 1 - i};
    }
  
  //Simulate wall kick
  while (tmpcorner.y < 0){
    tmpcorner.y++;
    for (int i = 0; i < NCOORDS; ++i)
      tmp[i].y++;
  }
  while (tmpcorner.y + piece.blength - 1 >= NCOL){
    tmpcorner.y--;
    for (int i = 0; i < NCOORDS; ++i)
      tmp[i].y--;
  }

  //Set rotated piece only if no collision is detected
  if (!isCollision(tmp)){
    for (int i = 0 ; i < NCOORDS; ++i){ 
      cells[piece.pos[i].x][piece.pos[i].y].off();
    }
    memcpy(piece.pos, tmp, sizeof(piece.pos));
    piece.bcorner = tmpcorner;
    color *col = &piece.Color;
    set(col->r,col->g,col->b,col->a);
  }

}


void Grid::free(){
  //Free cells
  for (int i = 0; i < NROW; ++i)
    delete [] cells[i];
  delete[] cells;

}

















