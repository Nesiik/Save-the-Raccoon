#ifndef WORLD_H
#define WORLD_H

enum GameState {
    Menu = -1, 
    Alive = 0, 
    Dead = 1,   
    Win = 2,    
    Quit = 3
};

struct world_s{
    sprite_t player;
    int level;
    char gameover; /*!< -1 = Menu; 0 = alive; 1 = Dead; 2 = Win; 3 = Quit */
};
typedef struct world_s world_t;

void init_data(world_t * world);
int fin(world_t* world);
int is_game_over(world_t *world);

#endif