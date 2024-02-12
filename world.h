#ifndef WORLD_H
#define WORLD_H

#include "ressources.h"

#define RESULT_FILE "../result/result.txt"
#define NB_LEVELS 3
#define DIRT_SIZE 32
#define TREE_SIZE 96
#define N_WIDTH_DIRT_SPRITE 6
#define N_HEIGHT_DIRT_SPRITE 5

enum GameState {
    Menu = -1,
    Alive = 0,
    Dead = 1,
    Win = 2,
    Finished = 3, 
    Quit = 4
};

typedef struct level_s {
    char** level_tab;
    int nb_ligne_level_tab;
    int nb_col_level_tab;
}level_t;

typedef struct world_s {
    int player_spawn_x,player_spawn_y;
    char need_player_pos_update;
    Uint64 start_level_time,end_level_time;
    double* levels_times; /* list of all the time for completing each level */
    int cur_level;
    int last_level; /* Level before pause */
    char game_state; /* -1 = Menu; 0 = alive; 1 = Dead; 2 = Win; 3 = Quit */
    level_t** levels; /* list of level pointer */
}world_t;

world_t* init_world();
void world_event(SDL_Event *event,world_t *world , ressources_t* ressources);
void mouse_menu_events(SDL_MouseButtonEvent button,world_t* world, ressources_t* ressources);
void render_sky(SDL_Renderer* renderer, ressources_t* ressources);
void render_worlds(SDL_Renderer* renderer,ressources_t* ressources,world_t* world);
void render_main_menu_text(SDL_Renderer *renderer,ressources_t *ressources);
char get(world_t* world,const int i,const int j);
char is_empty(world_t* world,const int i, const int j);
char get_collision(world_t* world,const int i, const int j);
void spike_collision(world_t* world);
void flag_collision(world_t* world);
void death(SDL_Renderer* renderer, world_t* world,ressources_t* ressources);
void won(SDL_Renderer* renderer,world_t* world,ressources_t* ressources);
void free_levels(world_t* world);
int fin(world_t* world);

#endif