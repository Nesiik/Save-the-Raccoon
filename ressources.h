#ifndef RESSOURCES_H
#define RESSOURCES_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "sprite.h"

#define MAIN_MENU_ITEM_COUNT 3
#define PLAYER_SPRITE_SIZE 64

typedef struct Item_s{
    SDL_Rect rect; /*!<Les données utiles pour l'affichage du texte */
    char* text; /*!<Le texte du menu */
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
    sprite* player;
}ressources_t;

ressources_t* init_ressources(SDL_Renderer *renderer);
void free_ressources(ressources_t* ressources);

#endif