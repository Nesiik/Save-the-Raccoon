#ifndef SDL_H
#define SDL_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "world.h"
#include "player.h"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define MAIN_MENU_ITEM_COUNT 3


typedef struct {
    SDL_Rect rect; /*!<Les données utiles pour l'affichage du texte*/
    char* text; /*!<Le texte du menu*/
} Item;


typedef struct {
    Item ItemList[MAIN_MENU_ITEM_COUNT];
    unsigned char selectedItem;
} MenuItem;

typedef struct ressources_s{
    MenuItem MenuItems;
    TTF_Font* font;
    sprite* player;
    sprite* background;
}ressources_t;

void init_ressources(SDL_Renderer *renderer, ressources_t* ressources);
SDL_Window* create_window();
SDL_Renderer* create_renderer(SDL_Window* window);
void handle_events(SDL_Event *event,world_t *world , ressources_t* ressources, player_t* player);
//void render_player(SDL_Renderer* renderer, ressources_t * ressources, player_t* player);
void afficher_texte(SDL_Renderer* renderer, TTF_Font* police, const char text[], int x, int y );
void render_main_menu_text(SDL_Renderer *renderer,ressources_t *ressources);
void render_main_menu_background(SDL_Renderer* renderer,ressources_t* ressources,world_t* world);
void free_ressources(ressources_t* ressources);

#endif