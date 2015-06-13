#ifndef GUARD_grid_h
#define GUARD_grid_h

//Size of nxn cells
#define SIZE 20

//Grid dimensions
#define NROW 22
#define NCOL 22

//Cells for tetris grid
class Cell{
  public:
    Cell();
    Cell(int, int);
    ~Cell();
    void init(int, int);
    void free();
    SDL_Rect getRect();

  private:
    SDL_Rect rect;
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

    //get nth row of cell grid
    SDL_Rect* getRow(int);

    //Deallocates texture
    void free();

  private:
    //Tetris grid composed of 22x10 cells
    Cell** cells;
};



#endif
