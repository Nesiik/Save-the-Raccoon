#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include "sprite.h"
#include "world.h"

#define PLAYER_SPRITE_SIZE 64
#define SPEED 10

enum States { REST=0, TAKEOFF=1, FLIGHT=2, LANDING=3, WALK=4, FALL=5 };

typedef struct player_s{
    double x,y;
    double vx,vy;
    Uint64 time;
    int state;
    sprite* sprite;
    unsigned char backward;
}player_t;

player_t* init_player(SDL_Renderer *renderer);
//void set_state(player_t* player,unsigned char state,unsigned char back);
void render_player(SDL_Renderer* renderer, player_t* player);
void deplacement(player_t* player,world_t* world, SDL_Keycode code);
void free_player(player_t* player);

#endif