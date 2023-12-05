#ifndef SDL_H
#define SDL_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "world.h"
#include "player.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define MAIN_MENU_ITEM_COUNT 3
#define N_WIDTH_DIRT_SPRITE 6
#define N_HEIGHT_DIRT_SPRITE 5

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
}ressources_t;

ressources_t* init_ressources(SDL_Renderer *renderer);
SDL_Window* create_window();
SDL_Renderer* create_renderer(SDL_Window* window);
void handle_events(SDL_Event *event,world_t *world , ressources_t* ressources, player_t* player);
void afficher_texte(SDL_Renderer* renderer, TTF_Font* police, const char text[], int x, int y );
SDL_Surface* texte_surface(TTF_Font* police,const char text[],char type,SDL_Color* color);
SDL_Texture* creer_texte_texture(SDL_Renderer* renderer, TTF_Font* police,char type,SDL_Color* color, const char text[],int x, int y, SDL_Rect* info);
void render_main_menu_text(SDL_Renderer *renderer,ressources_t *ressources);
void render_sky(SDL_Renderer* renderer, ressources_t* ressources);
void render_worlds(SDL_Renderer* renderer,ressources_t* ressources,world_t* world);
void free_ressources(ressources_t* ressources);

#endif