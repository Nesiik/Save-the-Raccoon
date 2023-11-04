#ifndef SDL_H
#define SDL_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "world.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define MAIN_MENU_ITEM_COUNT 3


typedef struct {
    SDL_Rect rect; /*!<Les données utiles pour l'affichage du texte*/
    const char* text; /*!<Le texte du menu*/
} Item;


typedef struct {
    Item ItemList[MAIN_MENU_ITEM_COUNT];
    char selectedItem;
} MenuItem;

typedef struct ressources_s{
    MenuItem MenuItems;
    TTF_Font* font;
}ressources_t;

enum FullScreenState {
    None = 0
};

void init_ressources(SDL_Renderer *renderer, ressources_t* ressources);
SDL_Window* create_window();
SDL_Renderer* create_renderer(SDL_Window* window);
void handle_events(SDL_Event *event,world_t *world , ressources_t* ressources);
void afficher_texte(SDL_Renderer* renderer, TTF_Font* police, const char text[], int x, int y );
void render_menu(SDL_Renderer *renderer,ressources_t *ressources);

#endif