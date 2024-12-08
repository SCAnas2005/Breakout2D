
#include <SDL.h>

#ifndef __FUNCTIONS__C__
#define __FUNCTIONS__C__

    enum bool{
        true = 1, false = 0
    };  
    typedef enum bool bool;

    struct Wall{
        SDL_Rect rect;
        bool valid;
    };
    typedef struct Wall Wall;

    void setError(char *message);
    void DestroyWindowAndRenderer(SDL_Window *window, SDL_Renderer *render);
    void showBackgroundColor(SDL_Window *window, SDL_Renderer *render, int r, int g, int b, int a);


    void drawBall(SDL_Window *window, SDL_Renderer *render, int x, int y, int c);
    void drawPlayer(SDL_Window *window, SDL_Renderer *render, int x, int w, int h);

    bool collision(int xBall, int yBall, int ballHeight, int XPlayer, int playerWidth);

    SDL_Rect *createListRectsWalls(int wallWidth, int wallHeight, int spaceBetweenWallsX, int spaceBetweenWallsY);
    void drawWalls(SDL_Window *window, SDL_Renderer *render, SDL_Rect walls[], int validWalls[]);
    int *createValidWalls(void);
    int collisionWall(int XBall, int yBall, SDL_Rect *walls);

    bool isWin(int validWalls[]);

#endif