#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Rect rect; /*!<Les données utiles pour l'affichage du texte*/
    const char* text; /*!<Le texte du menu*/
} Item;


typedef struct {
    Item ItemList[3];
    char numMenuItems;
    char selectedItem;
} MenuItem;

struct ressources_s{
    MenuItem MenuItems;
};
typedef struct ressources_s ressources_t;

enum FullScreenState {
    None = 0
};

int init_sdl(SDL_Window **window, SDL_Renderer **renderer, int width, int height);

void init_ressources(SDL_Renderer *renderer, ressources_t* ressources);

#endif