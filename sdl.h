#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include "world.h"

typedef struct {
    SDL_Rect rect; /*!<Les données utiles pour l'affichage du texte*/
    const char* text; /*!<Le texte du menu*/
} Item;


typedef struct {
    Item ItemList[3];
    char numMenuItems;
    char selectedItem;
} MenuItem;

typedef struct ressources_s{
    MenuItem MenuItems;
}ressources_t;

enum FullScreenState {
    None = 0
};

void init_ressources(SDL_Renderer *renderer, ressources_t* ressources);
SDL_Window* create_window();
SDL_Renderer* create_renderer(SDL_Window* window);
void handle_events(SDL_Event *event,world_t *world);

#endif