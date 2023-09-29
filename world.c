#include "world.h"

void init_data(world_t * world){
    world->gameover = Menu;
}

int is_game_over(world_t *world){
    if(world->gameover > Alive) // > 0
        return 1;
    else
        return 0;
}

int fin(world_t* world){
    if(world->gameover  == ForceQuit)
        return 1;
    else
        return 0;
}