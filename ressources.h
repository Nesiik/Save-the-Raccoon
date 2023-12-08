#ifndef RESSOURCES_H
#define RESSOURCES_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "sprite.h"
#include "sdl.h"
#define MAIN_MENU_ITEM_COUNT 3

typedef struct Item_s{
    SDL_Rect rect; /*!<Les données utiles pour l'affichage du texte*/
    char* text; /*!<Le texte du menu*/
    SDL_Texture* texture; 
} Item;


typedef struct MenuItem_s{
    Item ItemList[MAIN_MENU_ITEM_COUNT];
    signed char curselectedItem;
    signed char lastselectedItem;
} MenuItem_t;

typedef struct ressources_s{
    MenuItem_t* MenuItems;
    TTF_Font* font;
    sprite* dirt;
    sprite* spike;
    sprite* sky;
    sprite* tree;
    sprite* flag;
    sprite* coin;
}ressources_t;

ressources_t* init_ressources(SDL_Renderer *renderer);
void free_ressources(ressources_t* ressources);
void render_main_menu_text(SDL_Renderer *renderer,ressources_t *ressources);

#endif