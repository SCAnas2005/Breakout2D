
/* 
            |||       ==========     |||           |||
            |||      |||             |||           |||
            |||      |||             |||           |||
    ___     |||      |||======       |||           |||
    |||     |||      |||             |||           |||
    \\\     ///      |||              \\\         ///
       =====          ==========        ===========
*/ 

#include <stdio.h>
#include <SDL.h>
#include "functions.h"
#include "variables.h"


int main(int argv, char **argc)
{
    // ======================================================================================================
    SDL_Window *window = NULL; // Pointeur fenetre
    SDL_Renderer *render = NULL; // Pointeur rendu

    int time = 0;
    int seconds = 0;

    SDL_Rect *walls = malloc(12*sizeof(SDL_Rect));                                               // Création 
    if (walls == NULL) {printf("...\n"); exit(0);}                                               // liste des 
    walls = createListRectsWalls(wallWidth, wallHeight, spaceBetweenWallsX, spaceBetweenWallsY); // rect des murs

    int *validWalls = malloc(12*sizeof(int)); 
    validWalls = createValidWalls();                   // Element a afficher ou pas (0 ou 1)
    if (validWalls == NULL) {printf("===\n"); exit(0);}


    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) // Chargement des modules SDL
    {
        setError("SDL_Init()");
    }
    // ======================================================================================================

    // ======================================================================================================
    printf("Debut du programme...\n");
    window = SDL_CreateWindow("2D Game", 
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
    WIN_WIDTH, WIN_HEIGHT, 0);
    if (window == NULL) {setError("SDL_CreateWindow()");}

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE); // Création du rendu
    if (render == NULL) {SDL_DestroyWindow(window); setError("SDL_CreateRenderer()");}
    // ======================================================================================================

    if (SDL_WarpMouseGlobal(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED) != 0)
    {
        DestroyWindowAndRenderer(window, render);
        setError("SDL_WarpMouseGlobal()");
    }
    
    SDL_bool started = SDL_TRUE;
    while (started)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    printf("Fin du programme...");
                    started = SDL_FALSE;
                    break;
                
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_RIGHT:
                        if (playerX+dPlayerX+playerWidth <= WIN_WIDTH)
                            playerX += dPlayerX;
                        break;
                    case SDLK_LEFT:
                        if (playerX-dPlayerX >= 0)
                            playerX -= dPlayerX;
                        break;
                    
                    default:
                        break;
                    }
                    break;
                
                case SDL_MOUSEMOTION:
                    playerX = event.motion.x-(playerWidth/2);
                    break;
                
                default:
                    break;
            }
        }

        // ===================================================================================
        showBackgroundColor(window, render, 159,236,221, 255); // Background
        drawBall(window, render, xBall, yBall, side);
        drawPlayer(window, render, playerX, playerWidth, playerHeight);
        drawWalls(window, render, walls, validWalls);

        SDL_RenderPresent(render); // Rafraichissement écran
        // ===================================================================================


        // ===================================================================================
        if (xBall+dXBall < 0 || xBall+dXBall+side > WIN_WIDTH)
        {
            dXBall = -dXBall;
        }
        if (yBall+dYBall < 0 || collision(xBall, yBall, side, playerX, playerWidth)) {dYBall = -dYBall;}

        if (yBall+dYBall+side > WIN_HEIGHT) // Touche le sol
        {
            printf("You lose !!\n");
            started = SDL_FALSE;
        }

        int i = collisionWall(xBall, yBall, walls);
        if (i != -1) // Verifie et supprimer mur en cas de colision.
        {
            dYBall = -dYBall;
            validWalls[i] = 0;
            walls[i].w = 0; 
        }

        xBall += dXBall; yBall += dYBall; // Avance de la balle

        if (isWin(validWalls)) {started = SDL_FALSE; printf("Gagnée... !!\n");} // victoire
        // ===================================================================================

        SDL_Delay(10); // Vitesse de la boucle. 
        time += 10;
        if (time == 1000)
        {
            time = 0;
            seconds++;
            printf("00:0%d\n", seconds);
        }
    }

    free(walls);                // Libère mémoire des pointeurs vers murs.
    free(validWalls);           // Libère mémoire des pointeurs vers murs valides
    DestroyWindowAndRenderer(window, render);  
    SDL_Quit(); // Déchargement
    return EXIT_SUCCESS;
}
