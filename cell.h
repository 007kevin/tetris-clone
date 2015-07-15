#ifndef GUARD_cell_h
#define GUARD_cell_h

//Size of nxn cells
#define SIZE 20

/*********************************
 * Cells for tetris grid
 * ******************************/
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

    //Make cell visible with specified colors
    void on(Uint8, Uint8, Uint8, Uint8);

    //Make cell invisible
    void off();
    
    //Get current state of the cell
    bool getStatus();

    //Get colors
    Uint8 getr(); 
    Uint8 getg();
    Uint8 getb();
    Uint8 geta();

  private:
    //Cell dimensions
    SDL_Rect rect;
    //Cell colors
    Uint8 r, g, b, a;
    //True indicates cell is on
    bool status;
};

#endif
