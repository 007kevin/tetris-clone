#ifndef GUARD_grid_h
#define GUARD_grid_h

//Size of nxn cells
#define SIZE 20

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

//Cells for tetris grid
class Cell{
  public:
    Cell();
    Cell(int, int);
    ~Cell();

    //Initialize cell position
    void init(int, int);

    //Free cell memory
    void free();

    //Render cell
    void render();

    //Make cell visible
    void on();

    //Make cell invisible
    void off();

  private:
    //Cell dimensions
    SDL_Rect rect;
    //Cell colors
    Uint8 r, g, b, a;
};

//Grid for tetris
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
    
    //Set game piece to display before rendering
    void set();

    //Deallocates texture
    void free();

  private:
    //Tetris grid composed of 22x10 cells
    Cell** cells;
    //Tetris game piece which holds coordinates of the game piece
    coord* piece;
  
};



#endif
