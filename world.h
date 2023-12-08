#ifndef WORLD_H
#define WORLD_H

#include "sprite.h"
#include "ressources.h"

#define NB_LEVELS 2
#define DIRT_SIZE 32
#define TREE_SIZE 96
#define N_WIDTH_DIRT_SPRITE 6
#define N_HEIGHT_DIRT_SPRITE 5

enum GameState {
    Menu = -1, 
    Alive = 0, 
    Dead = 1,   
    Win = 2,    
    Quit = 3
};

typedef struct level_s
{
    char** level_tab;
    int nb_ligne_level_tab;
    int nb_col_level_tab;
}level;


struct world_s{
    int cur_level;
    int last_level; /* Level before pause */
    char game_state; /*!< -1 = Menu; 0 = alive; 1 = Dead; 2 = Win; 3 = Quit */
    level** levels; /* 0 = Menu , 1 = niveau 1 , etc... */
};
typedef struct world_s world_t;

void mouse_menu_events(SDL_MouseButtonEvent button,world_t* world, ressources_t* ressources);
world_t* init_world();
void render_sky(SDL_Renderer* renderer, ressources_t* ressources);
void render_worlds(SDL_Renderer* renderer,ressources_t* ressources,world_t* world);
char get(world_t* world,const int i,const int j);
char is_empty(world_t* world,const int i, const int j);
void free_levels(world_t* world);
int fin(world_t* world);
int is_game_over(world_t *world);

#endif