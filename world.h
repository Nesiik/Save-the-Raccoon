#ifndef WORLD_H
#define WORLD_H

#include "sprite.h"
#define NB_LEVELS 2
#define DIRT_SIZE 32

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
    char game_state; /*!< -1 = Menu; 0 = alive; 1 = Dead; 2 = Win; 3 = Quit */
    level** levels; /* 0 = Menu , 1 = niveau 1 , etc... */
};
typedef struct world_s world_t;

world_t* init_world();
char world_collision(world_t* world,SDL_Rect* pos);
int* get_dirt_level(world_t* world,SDL_Rect* pos);
char is_empty(world_t* world,const int i, const int j);
void free_levels(world_t* world);
int fin(world_t* world);
int is_game_over(world_t *world);

#endif