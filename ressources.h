#ifndef RESSOURCES_H
#define RESSOURCES_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "sprite.h"

#define MAIN_MENU_ITEM_COUNT 3
#define PLAYER_SPRITE_SIZE 63
#define FONT_SIZE 35
#define FONT_MULT 5

typedef struct Item_s{
    SDL_Rect rect; /*!<Les données utiles pour l'affichage du texte */
    char* text; /*!<Le texte du menu */
    SDL_Texture* texture;
} Item_t;


typedef struct MenuItem_s{
    Item_t** ItemList; /* linked list of item pointer */
    Item_t** curselectedItem; /* pointer to the selected item pointer (part of the itemList array) */
    Item_t** lastselectedItem; /* pointer to the last item pointer (part of the itemList array) */
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