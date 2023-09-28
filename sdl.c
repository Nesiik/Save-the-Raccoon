#include "sdl.h"

void init_ressources(SDL_Renderer *renderer, ressources_t* ressources){
    ressources->MenuItems.ItemList[0].rect = (SDL_Rect){100,200,0,0};
    ressources->MenuItems.ItemList[0].text = "Jouer";
    ressources->MenuItems.ItemList[1].rect = (SDL_Rect){100,250,0,0};
    ressources->MenuItems.ItemList[1].text = "Option";
    ressources->MenuItems.ItemList[2].rect = (SDL_Rect){100,300,0,0};
    ressources->MenuItems.ItemList[2].text = "Quitter";

    ressources->MenuItems.numMenuItems = 3;
    ressources->MenuItems.selectedItem = -1;
}

int init_sdl(SDL_Window **window, SDL_Renderer **renderer, int width, int height)
{
    if(0 != SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO))
    {
        fprintf(stderr, "Erreur initialisation de la SDL : %s", SDL_GetError());
        return -1;
    }
    if(0 != SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, window, renderer))
    {
        fprintf(stderr, "Erreur lors de la creation de l'image et du renderer : %s", SDL_GetError());
        return -1;
    }
    return 0;
}