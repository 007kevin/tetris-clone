#ifndef GUARD_grid_h
#define GUARD_grid_h

//Size of nxn cells
#define SIZE 20

//Grid dimensions
#define NROW 22
#define NCOL 10

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

    //Deallocates texture
    void free();

  private:
    //Tetris grid composed of 22x10 cells
    Cell** cells;
};



#endif
