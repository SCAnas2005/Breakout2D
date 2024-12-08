
#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>

#include "functions.h"

// Show an error when there are. 
void setError(char *message)
{
    SDL_Log("Error : %s > %s", message, SDL_GetError());
    exit(EXIT_FAILURE);
}

void DestroyWindowAndRenderer(SDL_Window *window, SDL_Renderer *render)
{
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
}

void showBackgroundColor(SDL_Window *window, SDL_Renderer *render, int r, int g, int b, int a)
{
    if (SDL_SetRenderDrawColor(render, r,g,b, a) != 0) 
    {
        DestroyWindowAndRenderer(window, render); 
        setError("showBackgroundColor()");
    }

    SDL_RenderClear(render); // Background avec sa couleur
}

void drawBall(SDL_Window *window, SDL_Renderer *render, int x, int y, int c)
{
    SDL_Rect ball = {x, y, c, c};

    if (SDL_SetRenderDrawColor(render, 255,0,0, 255) != 0)
    {
        DestroyWindowAndRenderer(window, render);
        setError("drawBall()");
    }

    if (SDL_RenderFillRect(render, &ball) != 0)
    {
        DestroyWindowAndRenderer(window, render);
        setError("drawBall()");
    }
}

void drawPlayer(SDL_Window *window, SDL_Renderer *render, int x, int w, int h)
{
    if (x > 1080-h) {x = 1080-h;} else if (x < 0) {x = 0;}
    SDL_Rect player = {x, 720-50, w, h};
    if (SDL_SetRenderDrawColor(render, 0,200,0, 255) != 0)
    {
        DestroyWindowAndRenderer(window, render);
        setError("drawPlayer()");
    }

    if (SDL_RenderFillRect(render, &player) != 0)
    {
        DestroyWindowAndRenderer(window, render);
        setError("drawPlayer()");
    }
}

SDL_Rect *createListRectsWalls(int wallWidth, int wallHeight, int spaceBetweenWallsX, int spaceBetweenWallsY) // Créer la liste des rects
{
    int x = spaceBetweenWallsX+40, y = spaceBetweenWallsY;
    SDL_Rect *wallsCoord = malloc(12*sizeof(Wall));
    if (wallsCoord == NULL)
    {
        printf("Erreur allocation memoire 1\n");
        exit(EXIT_FAILURE);
    }
    int n = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            SDL_Rect temp = {x, y, wallWidth, wallHeight};
            wallsCoord[n] = temp;
            x += (wallWidth+spaceBetweenWallsX);
            n++;
        }
        x = spaceBetweenWallsX+40;
        y += (wallHeight+spaceBetweenWallsY);
    }

    return wallsCoord;
}

int *createValidWalls(void) // Créer la liste des murs valides.
{
    int *validWalls = malloc(12*sizeof(int));
    if (validWalls == NULL) {printf("error: creazteValidWalls()\n"); exit(0);}
    for (int i = 0; i < 12; i++)
    {
        validWalls[i] = 1;
    }

    return validWalls;
}

void drawWalls(SDL_Window *window, SDL_Renderer *render, SDL_Rect *walls, int validWalls[]) // Dessin des murs.
{
    if (SDL_SetRenderDrawColor(render, 0,0,255, 255) != 0)
    {
        DestroyWindowAndRenderer(window, render);
        setError("drawWalls()");
    }

    for (int i = 0; i < 12; i++)
    {
        if (validWalls[i] && walls[i].w > 0)
        {
            if (SDL_RenderFillRect(render, &walls[i]) != 0)
            {
                DestroyWindowAndRenderer(window, render);
                setError("drawWalls()");
            }
        }
    }
}


bool collision(int xBall, int yBall, int ballHeight, int XPlayer, int playerWidth) // Colision entre balle et joueur
{
    if (xBall >= XPlayer && xBall <= XPlayer+playerWidth)
    {
        if (yBall+ballHeight == 720-50 || yBall+ballHeight == 720-50+2) return true;
    }
    return false;
}

int collisionWall(int XBall, int yBall, SDL_Rect *walls) // Colision balle/mur
{
    for (int i = 0; i < 12; i++)
    {
        if (XBall >= walls[i].x && XBall <= walls[i].x+walls[i].w)
        {
            if (yBall >= walls[i].y && yBall <= walls[i].y+2)
            {
                return i;
            }
        }
    }

    return -1;
}


bool isWin(int validWalls[]) // Vérification de victoire
{
    for (int i = 0; i < 12; i++)
    {
        if (validWalls[i]) {return false;}
    }

    return true;
}
