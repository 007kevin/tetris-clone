#ifndef GUARD_grid_h
#define GUARD_grid_h

#include "cell.h"

//Grid dimensions
#define NROW 22
#define NCOL 10

//Define starting cell coordinates for game pieces.
//Should be within the range of NROW and NCOL.
//Note: Cell coordinates do not follow the x, y 
//of window coorindates
#define STARTX 0
#define STARTY 3

//Number of coords per game piece
#define NCOORDS 4


//Struct containing defined game pieces
struct coord{
 int x,y;
};


//Enumeration for direction
enum Direction{
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT
};

/*********************************
 * Struct to hold color definition
 * ******************************/
struct color{
  Uint8 r, g, b, a;
};

/*********************************
 * Grid for tetris
 * ******************************/
class Grid{
  public:
    //Initialize variables
    Grid();

    //Deallocate memory
    ~Grid();

    //Initialize grid
    void init();
    
    //Render grid
    void render();

    //Load game piece
    void load();
    
    //Set game piece to display before rendering. Used in load()
    //and move()
    void set(Uint8, Uint8, Uint8, Uint8);

    //Move game piece if no collision and update
    //piece member of Grid class
    void move(int);

    //Check if moving in a direction results in a collision
    //with either the border or cell
    bool isCollision(int);

    //Check if given coordinated are part of the game piece. Also
    //checks if coordinates are outside game borders.
    //Used in isCollision()
    bool isGamePiece(int, int);

    //Check for complete lines. Delete and shift down if any found.
    //This function is called everytime a game piece bottom collision is 
    //detected.
    void shift();

    //Deallocates texture
    void free();

  private:
    //Tetris grid composed of 22x10 cells
    Cell** cells;
    //Tetris game piece which holds coordinates of the game piece
    coord* piece;
  
};



#endif
