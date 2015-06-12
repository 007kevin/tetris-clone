#ifndef GUARD_ltexture_h
#define GUARD_ltexture_h

//Texture wrapper class
class LTexture{
  public:
    //Initialize variables
    LTexture();

    //Deallocate memory
    ~LTexture();

    //Deallocates texture
    free();

    //Renders texture at a given point
    void render(int x, int y, SDL_Rect grid);

    //Gets grid dimension
    int getWidth();
    int getHeight();

  private:
    //The actual hardware texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
}

#endif
